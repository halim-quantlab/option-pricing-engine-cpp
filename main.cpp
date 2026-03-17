#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>

#include "include/option.hpp"
#include "include/black_scholes.hpp"
#include "include/monte_carlo.hpp"
#include "include/analysis.hpp"

std::string to_string(OptionType type) {
    if (type == OptionType::Call) return "Call";
    return "Put";
}

std::string to_string(PricingMethod method) {
    if (method == PricingMethod::BlackScholes) return "Black-Scholes";
    return "Monte Carlo";
}

std::string to_string(PayoffStyle style) {
    if (style == PayoffStyle::European) return "European";
    return "Asian";
}

double get_double_input(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a numeric value.\n";
        } else {
            return value;
        }
    }
}

int get_int_input(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter an integer.\n";
        } else {
            return value;
        }
    }
}

bool get_yes_no_input(const std::string& prompt) {
    char choice;
    while (true) {
        std::cout << prompt << " (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') return true;
        if (choice == 'n' || choice == 'N') return false;

        std::cout << "Invalid input. Please enter y or n.\n";
    }
}

PricingMethod get_pricing_method() {
    int choice;
    while (true) {
        std::cout << "\nSelect Pricing Method:\n";
        std::cout << "1. Black-Scholes\n";
        std::cout << "2. Monte Carlo\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (!std::cin.fail() && (choice == 1 || choice == 2)) {
            return (choice == 1) ? PricingMethod::BlackScholes : PricingMethod::MonteCarlo;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter 1 or 2.\n";
    }
}

PayoffStyle get_payoff_style(PricingMethod method) {
    if (method == PricingMethod::BlackScholes) {
        std::cout << "\nBlack-Scholes only supports European options.\n";
        return PayoffStyle::European;
    }

    int choice;
    while (true) {
        std::cout << "\nSelect Payoff Style:\n";
        std::cout << "1. European\n";
        std::cout << "2. Asian\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (!std::cin.fail() && (choice == 1 || choice == 2)) {
            return (choice == 1) ? PayoffStyle::European : PayoffStyle::Asian;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter 1 or 2.\n";
    }
}

OptionType get_option_type() {
    int choice;
    while (true) {
        std::cout << "\nSelect Option Type:\n";
        std::cout << "1. Call\n";
        std::cout << "2. Put\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (!std::cin.fail() && (choice == 1 || choice == 2)) {
            return (choice == 1) ? OptionType::Call : OptionType::Put;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter 1 or 2.\n";
    }
}

void validate_parameters(const OptionParameters& params) {
    if (params.S0 <= 0.0) throw std::invalid_argument("S0 must be positive.");
    if (params.K <= 0.0) throw std::invalid_argument("K must be positive.");
    if (params.sigma <= 0.0) throw std::invalid_argument("sigma must be positive.");
    if (params.T <= 0.0) throw std::invalid_argument("T must be positive.");

    if (params.pricing_method == PricingMethod::MonteCarlo && params.simulations <= 0) {
        throw std::invalid_argument("simulations must be positive for Monte Carlo.");
    }

    if (params.payoff_style == PayoffStyle::Asian && params.monitoring_steps <= 0) {
        throw std::invalid_argument("monitoring_steps must be positive for Asian options.");
    }
}

OptionParameters get_user_parameters() {
    OptionParameters params{};

    std::cout << "Enter Option Pricing Inputs\n";
    std::cout << "---------------------------\n";

    params.S0 = get_double_input("Enter initial stock price S0: ");
    params.K = get_double_input("Enter strike price K: ");
    params.r = get_double_input("Enter risk-free rate r: ");
    params.y = get_double_input("Enter dividend yield y: ");
    params.sigma = get_double_input("Enter volatility sigma: ");
    params.T = get_double_input("Enter time to maturity T: ");

    params.pricing_method = get_pricing_method();
    params.payoff_style = get_payoff_style(params.pricing_method);
    params.option_type = get_option_type();

    if (params.pricing_method == PricingMethod::MonteCarlo) {
        params.simulations = get_int_input("Enter number of Monte Carlo simulations: ");
        params.use_antithetic_variates = get_yes_no_input("Use antithetic variates");
    } else {
        params.simulations = 0;
        params.use_antithetic_variates = false;
    }

    if (params.payoff_style == PayoffStyle::Asian) {
        params.monitoring_steps = get_int_input("Enter number of monitoring steps: ");
    } else {
        params.monitoring_steps = 0;
    }

    return params;
}

void print_header(const OptionParameters& params) {
    std::cout << "\nOption Pricing Engine\n";
    std::cout << "---------------------\n";
    std::cout << "Method       : " << to_string(params.pricing_method) << "\n";
    std::cout << "Payoff Style : " << to_string(params.payoff_style) << "\n";
    std::cout << "Option Type  : " << to_string(params.option_type) << "\n";
    std::cout << "S0           : " << params.S0 << "\n";
    std::cout << "K            : " << params.K << "\n";
    std::cout << "r            : " << params.r << "\n";
    std::cout << "y            : " << params.y << "\n";
    std::cout << "sigma        : " << params.sigma << "\n";
    std::cout << "T            : " << params.T << "\n";

    if (params.pricing_method == PricingMethod::MonteCarlo) {
        std::cout << "Simulations  : " << params.simulations << "\n";
        std::cout << "Antithetic   : " << (params.use_antithetic_variates ? "On" : "Off") << "\n";
    }

    if (params.payoff_style == PayoffStyle::Asian) {
        std::cout << "Steps        : " << params.monitoring_steps << "\n";
    }
}

void print_convergence_table(const ConvergenceResult& conv_result) {
    std::cout << "\nMonte Carlo Convergence Analysis\n";
    std::cout << "--------------------------------\n";
    std::cout << "Black-Scholes Benchmark Price: " << conv_result.black_scholes_price << "\n\n";

    std::cout << std::left
              << std::setw(12) << "N"
              << std::setw(18) << "MC Price"
              << std::setw(18) << "Std Error"
              << std::setw(18) << "Abs Error vs BS"
              << "\n";

    std::cout << std::string(66, '-') << "\n";

    for (const auto& row : conv_result.rows) {
        std::cout << std::left
                  << std::setw(12) << row.simulations
                  << std::setw(18) << row.monte_carlo_price
                  << std::setw(18) << row.std_error
                  << std::setw(18) << row.abs_error_vs_bs
                  << "\n";
    }
}

int main() {
    try {
        std::cout << std::fixed << std::setprecision(6);

        OptionParameters params = get_user_parameters();
        validate_parameters(params);

        print_header(params);

        if (params.pricing_method == PricingMethod::BlackScholes) {
            BlackScholesResult bs = BlackScholes::full_result(params);

            std::cout << "\nBlack-Scholes Result\n";
            std::cout << "--------------------\n";
            std::cout << "Price        : " << bs.price << "\n";
            std::cout << "Delta        : " << bs.delta << "\n";
            std::cout << "Gamma        : " << bs.gamma << "\n";
            std::cout << "Vega         : " << bs.vega << "\n";
            std::cout << "Theta        : " << bs.theta << "\n";
            std::cout << "Rho          : " << bs.rho << "\n";

            ParityResult parity = BlackScholes::call_put_parity(params);

            std::cout << "\nCall-Put Parity Check\n";
            std::cout << "---------------------\n";
            std::cout << "Call Price   : " << parity.call_price << "\n";
            std::cout << "Put Price    : " << parity.put_price << "\n";
            std::cout << "LHS (C - P)  : " << parity.lhs << "\n";
            std::cout << "RHS          : " << parity.rhs << "\n";
            std::cout << "Difference   : " << parity.difference << "\n";
        }
        else if (params.pricing_method == PricingMethod::MonteCarlo) {
            MCSummary result = MonteCarlo::price(params);

            std::cout << "\nMonte Carlo Result\n";
            std::cout << "------------------\n";
            std::cout << "Price        : " << result.price << "\n";
            std::cout << "Std Error    : " << result.std_error << "\n";
            std::cout << "95% CI Low   : " << result.ci_low << "\n";
            std::cout << "95% CI High  : " << result.ci_high << "\n";

            if (params.payoff_style == PayoffStyle::European) {
                MCParityResult parity = MonteCarlo::call_put_parity(params);

                std::cout << "\nMonte Carlo Call-Put Parity Check\n";
                std::cout << "---------------------------------\n";
                std::cout << "Call Price   : " << parity.call_price << "\n";
                std::cout << "Call SE      : " << parity.call_std_error << "\n";
                std::cout << "Put Price    : " << parity.put_price << "\n";
                std::cout << "Put SE       : " << parity.put_std_error << "\n";
                std::cout << "LHS (C - P)  : " << parity.lhs << "\n";
                std::cout << "RHS          : " << parity.rhs << "\n";
                std::cout << "Difference   : " << parity.difference << "\n";

                ComparisonResult comp = Analysis::compare_black_scholes_vs_monte_carlo(params);

                std::cout << "\nBlack-Scholes vs Monte Carlo Comparison\n";
                std::cout << "---------------------------------------\n";
                std::cout << "Black-Scholes Price : " << comp.black_scholes_price << "\n";
                std::cout << "Monte Carlo Price   : " << comp.monte_carlo_price << "\n";
                std::cout << "Monte Carlo SE      : " << comp.monte_carlo_std_error << "\n";
                std::cout << "Absolute Error      : " << comp.abs_error << "\n";
                std::cout << "Relative Error (%)  : " << comp.rel_error_pct << "\n";

                std::vector<int> simulation_grid = {1000, 5000, 10000, 50000, 100000};
                ConvergenceResult conv_result = Analysis::monte_carlo_convergence(params, simulation_grid);
                print_convergence_table(conv_result);

                VarianceReductionResult vr = Analysis::compare_standard_vs_antithetic(params);

                std::cout << "\nVariance Reduction: Standard vs Antithetic\n";
                std::cout << "------------------------------------------\n";
                std::cout << "Standard MC Price       : " << vr.standard_price << "\n";
                std::cout << "Standard MC Std Error   : " << vr.standard_se << "\n";
                std::cout << "Antithetic MC Price     : " << vr.antithetic_price << "\n";
                std::cout << "Antithetic MC Std Error : " << vr.antithetic_se << "\n";
                std::cout << "SE Reduction (%)        : " << vr.se_reduction_pct << "\n";

                BenchmarkResult bench = Analysis::benchmark_pricing_engines(params);

                std::cout << "\nRuntime Benchmark\n";
                std::cout << "-----------------\n";
                std::cout << "Black-Scholes Runtime (us)          : " << bench.black_scholes_us << "\n";
                std::cout << "Monte Carlo Standard Runtime (ms)   : " << bench.monte_carlo_standard_ms << "\n";
                std::cout << "Monte Carlo Antithetic Runtime (ms) : " << bench.monte_carlo_antithetic_ms << "\n";
            }
        }
        else {
            throw std::invalid_argument("Unsupported pricing method.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << '\n';
    }

    return 0;
}