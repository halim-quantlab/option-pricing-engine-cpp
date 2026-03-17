#pragma once

#include "enums.hpp"
#include <vector>

struct OptionParameters {
    double S0;
    double K;
    double r;
    double y;
    double sigma;
    double T;
    int simulations;
    int monitoring_steps;

    bool use_antithetic_variates;

    OptionType option_type;
    PricingMethod pricing_method;
    PayoffStyle payoff_style;
};

struct MCSummary {
    double price;
    double std_error;
    double ci_low;
    double ci_high;
};

struct BlackScholesResult {
    double price;
    double delta;
    double gamma;
    double vega;
    double theta;
    double rho;
};

struct ParityResult {
    double call_price;
    double put_price;
    double lhs;
    double rhs;
    double difference;
};

struct MCParityResult {
    double call_price;
    double call_std_error;
    double put_price;
    double put_std_error;
    double lhs;
    double rhs;
    double difference;
};

struct ComparisonResult {
    double black_scholes_price;
    double monte_carlo_price;
    double monte_carlo_std_error;
    double abs_error;
    double rel_error_pct;
};

struct ConvergenceRow {
    int simulations;
    double monte_carlo_price;
    double std_error;
    double abs_error_vs_bs;
};

struct ConvergenceResult {
    double black_scholes_price;
    std::vector<ConvergenceRow> rows;
};

struct VarianceReductionResult {
    double standard_price;
    double standard_se;
    double antithetic_price;
    double antithetic_se;
    double se_reduction_pct;
};

struct BenchmarkResult {
    double black_scholes_us;
    double monte_carlo_standard_ms;
    double monte_carlo_antithetic_ms;
};