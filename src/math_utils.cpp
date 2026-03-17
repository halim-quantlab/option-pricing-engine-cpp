#include "../include/math_utils.hpp"
#include <cmath>
#include <algorithm>

namespace MathUtils {

    double normal_pdf(double x) {
        static const double INV_SQRT_2PI = 0.3989422804014327;
        return INV_SQRT_2PI * std::exp(-0.5 * x * x);
    }

    double normal_cdf(double x) {
        return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
    }
}