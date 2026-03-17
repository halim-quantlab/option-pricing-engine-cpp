#pragma once

#include "option.hpp"
#include <random>
#include <vector>

namespace MonteCarlo {

    double gbm_terminal_price(double S0, double r, double y, double sigma, double T, double Z);

    double compute_payoff(double value, double K, OptionType option_type);

    double simulate_european_underlying(
        const OptionParameters& params,
        std::mt19937& gen,
        std::normal_distribution<double>& dist
    );

    double simulate_asian_average_price(
        const OptionParameters& params,
        std::mt19937& gen,
        std::normal_distribution<double>& dist
    );

    double simulate_asian_average_price_with_given_normals(
        const OptionParameters& params,
        const std::vector<double>& normals
    );

    MCSummary mc_summary(const std::vector<double>& payoffs, double r, double T);

    MCSummary price_standard(const OptionParameters& params);
    MCSummary price_antithetic(const OptionParameters& params);
    MCSummary price(const OptionParameters& params);

    MCParityResult call_put_parity(const OptionParameters& base_params);

}