#include "../include/monte_carlo.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace MonteCarlo {

    double gbm_terminal_price(double S0, double r, double y, double sigma, double T, double Z) {
        return S0 * std::exp((r - y - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * Z);
    }

    double compute_payoff(double value, double K, OptionType option_type) {
        if (option_type == OptionType::Call) {
            return std::max(value - K, 0.0);
        } else {
            return std::max(K - value, 0.0);
        }
    }

    double simulate_european_underlying(
        const OptionParameters& params,
        std::mt19937& gen,
        std::normal_distribution<double>& dist
    ) {
        double Z = dist(gen);
        return gbm_terminal_price(params.S0, params.r, params.y, params.sigma, params.T, Z);
    }

    double simulate_asian_average_price(
        const OptionParameters& params,
        std::mt19937& gen,
        std::normal_distribution<double>& dist
    ) {
        if (params.monitoring_steps <= 0) {
            throw std::invalid_argument("monitoring_steps must be positive for Asian option pricing.");
        }

        double dt = params.T / params.monitoring_steps;
        double St = params.S0;
        double sum_prices = 0.0;

        for (int j = 0; j < params.monitoring_steps; ++j) {
            double Z = dist(gen);
            St = gbm_terminal_price(St, params.r, params.y, params.sigma, dt, Z);
            sum_prices += St;
        }

        return sum_prices / params.monitoring_steps;
    }

    double simulate_asian_average_price_with_given_normals(
        const OptionParameters& params,
        const std::vector<double>& normals
    ) {
        if (params.monitoring_steps <= 0) {
            throw std::invalid_argument("monitoring_steps must be positive for Asian option pricing.");
        }

        double dt = params.T / params.monitoring_steps;
        double St = params.S0;
        double sum_prices = 0.0;

        for (double Z : normals) {
            St = gbm_terminal_price(St, params.r, params.y, params.sigma, dt, Z);
            sum_prices += St;
        }

        return sum_prices / params.monitoring_steps;
    }

    MCSummary mc_summary(const std::vector<double>& payoffs, double r, double T) {
        if (payoffs.empty()) {
            throw std::invalid_argument("Payoff vector is empty.");
        }

        const int n = static_cast<int>(payoffs.size());
        const double discount_factor = std::exp(-r * T);

        double mean_payoff = std::accumulate(payoffs.begin(), payoffs.end(), 0.0) / n;

        double variance_sum = 0.0;
        for (double payoff : payoffs) {
            variance_sum += (payoff - mean_payoff) * (payoff - mean_payoff);
        }

        double sample_variance = (n > 1) ? variance_sum / (n - 1) : 0.0;
        double sample_std_dev = std::sqrt(sample_variance);

        double price = discount_factor * mean_payoff;
        double std_error = discount_factor * sample_std_dev / std::sqrt(n);
        double ci_low = price - 1.96 * std_error;
        double ci_high = price + 1.96 * std_error;

        return {price, std_error, ci_low, ci_high};
    }

    MCSummary price_standard(const OptionParameters& params) {
        if (params.simulations <= 0) {
            throw std::invalid_argument("Number of simulations must be positive.");
        }

        std::mt19937 gen(std::random_device{}());
        std::normal_distribution<double> dist(0.0, 1.0);

        std::vector<double> payoffs;
        payoffs.reserve(params.simulations);

        for (int i = 0; i < params.simulations; ++i) {
            double underlying_value = 0.0;

            if (params.payoff_style == PayoffStyle::European) {
                underlying_value = simulate_european_underlying(params, gen, dist);
            }
            else if (params.payoff_style == PayoffStyle::Asian) {
                underlying_value = simulate_asian_average_price(params, gen, dist);
            }
            else {
                throw std::invalid_argument("Unsupported payoff style.");
            }

            double payoff = compute_payoff(underlying_value, params.K, params.option_type);
            payoffs.push_back(payoff);
        }

        return mc_summary(payoffs, params.r, params.T);
    }

    MCSummary price_antithetic(const OptionParameters& params) {
        if (params.simulations <= 0) {
            throw std::invalid_argument("Number of simulations must be positive.");
        }

        std::mt19937 gen(std::random_device{}());
        std::normal_distribution<double> dist(0.0, 1.0);

        std::vector<double> payoffs;
        payoffs.reserve(params.simulations);

        int pairs = params.simulations / 2;

        for (int i = 0; i < pairs; ++i) {
            if (params.payoff_style == PayoffStyle::European) {
                double Z = dist(gen);

                double ST_plus = gbm_terminal_price(params.S0, params.r, params.y, params.sigma, params.T, Z);
                double ST_minus = gbm_terminal_price(params.S0, params.r, params.y, params.sigma, params.T, -Z);

                double payoff_plus = compute_payoff(ST_plus, params.K, params.option_type);
                double payoff_minus = compute_payoff(ST_minus, params.K, params.option_type);

                double paired_payoff = 0.5 * (payoff_plus + payoff_minus);
                payoffs.push_back(paired_payoff);
            }
            else if (params.payoff_style == PayoffStyle::Asian) {
                std::vector<double> normals;
                normals.reserve(params.monitoring_steps);

                for (int j = 0; j < params.monitoring_steps; ++j) {
                    normals.push_back(dist(gen));
                }

                std::vector<double> anti_normals;
                anti_normals.reserve(params.monitoring_steps);
                for (double z : normals) {
                    anti_normals.push_back(-z);
                }

                double avg_plus = simulate_asian_average_price_with_given_normals(params, normals);
                double avg_minus = simulate_asian_average_price_with_given_normals(params, anti_normals);

                double payoff_plus = compute_payoff(avg_plus, params.K, params.option_type);
                double payoff_minus = compute_payoff(avg_minus, params.K, params.option_type);

                double paired_payoff = 0.5 * (payoff_plus + payoff_minus);
                payoffs.push_back(paired_payoff);
            }
            else {
                throw std::invalid_argument("Unsupported payoff style.");
            }
        }

        if (payoffs.empty()) {
            throw std::invalid_argument("Not enough simulations for antithetic variates. Use at least 2.");
        }

        return mc_summary(payoffs, params.r, params.T);
    }

    MCSummary price(const OptionParameters& params) {
        if (params.use_antithetic_variates) {
            return price_antithetic(params);
        }
        return price_standard(params);
    }

    MCParityResult call_put_parity(const OptionParameters& base_params) {
        if (base_params.payoff_style != PayoffStyle::European) {
            throw std::invalid_argument("Monte Carlo call-put parity is only supported for European options.");
        }

        OptionParameters call_params = base_params;
        call_params.option_type = OptionType::Call;

        OptionParameters put_params = base_params;
        put_params.option_type = OptionType::Put;

        MCSummary call_result = price(call_params);
        MCSummary put_result = price(put_params);

        double lhs = call_result.price - put_result.price;
        double rhs = base_params.S0 * std::exp(-base_params.y * base_params.T)
                   - base_params.K * std::exp(-base_params.r * base_params.T);

        return {
            call_result.price,
            call_result.std_error,
            put_result.price,
            put_result.std_error,
            lhs,
            rhs,
            lhs - rhs
        };
    }

}