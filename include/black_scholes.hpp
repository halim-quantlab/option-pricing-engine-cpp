#pragma once

#include "option.hpp"

namespace BlackScholes {
    double compute_d1(const OptionParameters& params);
    double compute_d2(const OptionParameters& params);

    double price(const OptionParameters& params);

    double delta(const OptionParameters& params);
    double gamma(const OptionParameters& params);
    double vega(const OptionParameters& params);
    double theta(const OptionParameters& params);
    double rho(const OptionParameters& params);

    BlackScholesResult full_result(const OptionParameters& params);
    ParityResult call_put_parity(const OptionParameters& base_params);
}