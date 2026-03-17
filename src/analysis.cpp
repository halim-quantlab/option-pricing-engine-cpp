#include "../include/analysis.hpp"
#include "../include/black_scholes.hpp"
#include "../include/monte_carlo.hpp"

#include <cmath>
#include <stdexcept>
#include <chrono>

namespace Analysis {
    VarianceReductionResult compare_standard_vs_antithetic(const OptionParameters& params) {
    OptionParameters standard_params = params;
    standard_params.use_antithetic_variates = false;

    OptionParameters antithetic_params = params;
    antithetic_params.use_antithetic_variates = true;

    MCSummary standard_result = MonteCarlo::price_standard(standard_params);
    MCSummary antithetic_result = MonteCarlo::price_antithetic(antithetic_params);

    double se_reduction_pct = 0.0;
    if (standard_result.std_error > 1e-12) {
        se_reduction_pct =
            (standard_result.std_error - antithetic_result.std_error)
            / standard_result.std_error * 100.0;
        }

    return {
        standard_result.price,
        standard_result.std_error,
        antithetic_result.price,
        antithetic_result.std_error,
        se_reduction_pct
        };
    }
    ComparisonResult compare_black_scholes_vs_monte_carlo(const OptionParameters& params) {
        if (params.payoff_style != PayoffStyle::European) {
            throw std::invalid_argument("Black-Scholes vs Monte Carlo comparison only supports European options.");
        }

        double bs_price = BlackScholes::price(params);
        MCSummary mc_result = MonteCarlo::price(params);

        double abs_error = std::fabs(mc_result.price - bs_price);

        double rel_error_pct = 0.0;
        if (std::fabs(bs_price) > 1e-12) {
            rel_error_pct = abs_error / std::fabs(bs_price) * 100.0;
        }

        return {
            bs_price,
            mc_result.price,
            mc_result.std_error,
            abs_error,
            rel_error_pct
        };
    }

    ConvergenceResult monte_carlo_convergence(
        const OptionParameters& base_params,
        const std::vector<int>& simulation_grid
    ) {
        if (base_params.payoff_style != PayoffStyle::European) {
            throw std::invalid_argument("Monte Carlo convergence analysis only supports European options.");
        }

        double bs_price = BlackScholes::price(base_params);

        ConvergenceResult result;
        result.black_scholes_price = bs_price;

        for (int n : simulation_grid) {
            if (n <= 0) {
                throw std::invalid_argument("Simulation counts must be positive.");
            }

            OptionParameters temp_params = base_params;
            temp_params.simulations = n;

            MCSummary mc_result = MonteCarlo::price(temp_params);

            ConvergenceRow row;
            row.simulations = n;
            row.monte_carlo_price = mc_result.price;
            row.std_error = mc_result.std_error;
            row.abs_error_vs_bs = std::fabs(mc_result.price - bs_price);

            result.rows.push_back(row);
        }

        return result;
    }
    BenchmarkResult benchmark_pricing_engines(const OptionParameters& params) {
        OptionParameters bs_params = params;
        bs_params.pricing_method = PricingMethod::BlackScholes;
        bs_params.payoff_style = PayoffStyle::European;
        bs_params.use_antithetic_variates = false;

        OptionParameters mc_standard_params = params;
        mc_standard_params.pricing_method = PricingMethod::MonteCarlo;
        mc_standard_params.use_antithetic_variates = false;

        OptionParameters mc_antithetic_params = params;
        mc_antithetic_params.pricing_method = PricingMethod::MonteCarlo;
        mc_antithetic_params.use_antithetic_variates = true;

        auto bs_start = std::chrono::high_resolution_clock::now();
        volatile double bs_price = BlackScholes::price(bs_params);
        auto bs_end = std::chrono::high_resolution_clock::now();

        auto mc_std_start = std::chrono::high_resolution_clock::now();
        volatile MCSummary mc_std_result = MonteCarlo::price_standard(mc_standard_params);
        auto mc_std_end = std::chrono::high_resolution_clock::now();

        auto mc_anti_start = std::chrono::high_resolution_clock::now();
        volatile MCSummary mc_anti_result = MonteCarlo::price_antithetic(mc_antithetic_params);
        auto mc_anti_end = std::chrono::high_resolution_clock::now();

        double black_scholes_us =
            std::chrono::duration<double, std::micro>(bs_end - bs_start).count();

        double monte_carlo_standard_ms =
            std::chrono::duration<double, std::milli>(mc_std_end - mc_std_start).count();

        double monte_carlo_antithetic_ms =
            std::chrono::duration<double, std::milli>(mc_anti_end - mc_anti_start).count();

        return {
            black_scholes_us,
            monte_carlo_standard_ms,
            monte_carlo_antithetic_ms
        };
    }
}