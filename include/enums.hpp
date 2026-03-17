#pragma once

enum class OptionType {
    Call,
    Put
};

enum class PricingMethod {
    BlackScholes,
    MonteCarlo
};

enum class PayoffStyle {
    European,
    Asian
};