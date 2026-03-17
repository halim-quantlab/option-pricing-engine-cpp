#include "../include/black_scholes.hpp"
#include "../include/math_utils.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackScholes {

    double compute_d1(const OptionParameters& params) {
        if (params.S0 <= 0.0 || params.K <= 0.0 || params.sigma <= 0.0 || params.T <= 0.0) {
            throw std::invalid_argument("S0, K, sigma, and T must be positive.");
        }

        return (
            std::log(params.S0 / params.K) +
            (params.r - params.y + 0.5 * params.sigma * params.sigma) * params.T
        ) / (params.sigma * std::sqrt(params.T));
    }

    double compute_d2(const OptionParameters& params) {
        return compute_d1(params) - params.sigma * std::sqrt(params.T);
    }

    double price(const OptionParameters& params) {
        if (params.payoff_style != PayoffStyle::European) {
            throw std::invalid_argument("Black-Scholes only supports European options.");
        }

        double d1 = compute_d1(params);
        double d2 = compute_d2(params);

        double discounted_spot = params.S0 * std::exp(-params.y * params.T);
        double discounted_strike = params.K * std::exp(-params.r * params.T);

        if (params.option_type == OptionType::Call) {
            return discounted_spot * MathUtils::normal_cdf(d1)
                 - discounted_strike * MathUtils::normal_cdf(d2);
        } else {
            return discounted_strike * MathUtils::normal_cdf(-d2)
                 - discounted_spot * MathUtils::normal_cdf(-d1);
        }
    }

    double delta(const OptionParameters& params) {
        double d1 = compute_d1(params);

        if (params.option_type == OptionType::Call) {
            return std::exp(-params.y * params.T) * MathUtils::normal_cdf(d1);
        } else {
            return std::exp(-params.y * params.T) * (MathUtils::normal_cdf(d1) - 1.0);
        }
    }

    double gamma(const OptionParameters& params) {
        double d1 = compute_d1(params);

        return std::exp(-params.y * params.T) * MathUtils::normal_pdf(d1)
             / (params.S0 * params.sigma * std::sqrt(params.T));
    }

    double vega(const OptionParameters& params) {
        double d1 = compute_d1(params);

        return params.S0 * std::exp(-params.y * params.T)
             * MathUtils::normal_pdf(d1) * std::sqrt(params.T);
    }

    double theta(const OptionParameters& params) {
        double d1 = compute_d1(params);
        double d2 = compute_d2(params);

        double first_term =
            - (params.S0 * std::exp(-params.y * params.T) * MathUtils::normal_pdf(d1) * params.sigma)
            / (2.0 * std::sqrt(params.T));

        if (params.option_type == OptionType::Call) {
            double second_term =
                - params.r * params.K * std::exp(-params.r * params.T) * MathUtils::normal_cdf(d2);

            double third_term =
                + params.y * params.S0 * std::exp(-params.y * params.T) * MathUtils::normal_cdf(d1);

            return first_term + second_term + third_term;
        } else {
            double second_term =
                + params.r * params.K * std::exp(-params.r * params.T) * MathUtils::normal_cdf(-d2);

            double third_term =
                - params.y * params.S0 * std::exp(-params.y * params.T) * MathUtils::normal_cdf(-d1);

            return first_term + second_term + third_term;
        }
    }

    double rho(const OptionParameters& params) {
        double d2 = compute_d2(params);

        if (params.option_type == OptionType::Call) {
            return params.K * params.T * std::exp(-params.r * params.T) * MathUtils::normal_cdf(d2);
        } else {
            return -params.K * params.T * std::exp(-params.r * params.T) * MathUtils::normal_cdf(-d2);
        }
    }

    BlackScholesResult full_result(const OptionParameters& params) {
        if (params.payoff_style != PayoffStyle::European) {
            throw std::invalid_argument("Black-Scholes Greeks only support European options.");
        }

        return {
            price(params),
            delta(params),
            gamma(params),
            vega(params),
            theta(params),
            rho(params)
        };
    }

    ParityResult call_put_parity(const OptionParameters& base_params) {
        if (base_params.payoff_style != PayoffStyle::European) {
            throw std::invalid_argument("Call-put parity is only valid here for European options.");
        }

        OptionParameters call_params = base_params;
        call_params.option_type = OptionType::Call;

        OptionParameters put_params = base_params;
        put_params.option_type = OptionType::Put;

        double call_price = price(call_params);
        double put_price = price(put_params);

        double lhs = call_price - put_price;
        double rhs = base_params.S0 * std::exp(-base_params.y * base_params.T)
                   - base_params.K * std::exp(-base_params.r * base_params.T);

        return {
            call_price,
            put_price,
            lhs,
            rhs,
            lhs - rhs
        };
    }

}