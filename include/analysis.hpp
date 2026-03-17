#pragma once

#include "option.hpp"
#include <vector>

namespace Analysis {

    ComparisonResult compare_black_scholes_vs_monte_carlo(const OptionParameters& params);

    ConvergenceResult monte_carlo_convergence(
        const OptionParameters& base_params,
        const std::vector<int>& simulation_grid
    );

    VarianceReductionResult compare_standard_vs_antithetic(const OptionParameters& params);

    BenchmarkResult benchmark_pricing_engines(const OptionParameters& params);

}