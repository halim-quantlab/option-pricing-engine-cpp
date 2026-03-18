# 📈 Option Pricing Engine (C++)

A modular C++ option pricing engine implementing analytical and numerical methods for derivative pricing, including Black-Scholes, Monte Carlo simulation, variance reduction techniques, and performance benchmarking.

---

## 🚀 Features

### 📊 Pricing Models
- Black-Scholes (closed-form)
- Monte Carlo Simulation
  - European options
  - Asian options (arithmetic average)

---

### ⚙️ Advanced Monte Carlo
- Standard Monte Carlo
- Antithetic Variates (variance reduction)
- Confidence Intervals (95%)
- Standard Error estimation

---

### 📐 Greeks (Black-Scholes)
- Delta
- Gamma
- Vega
- Theta
- Rho

---

### 🔍 Validation & Analysis
- Call-Put Parity (Black-Scholes & Monte Carlo)
- Black-Scholes vs Monte Carlo comparison
- Monte Carlo convergence analysis

---

### ⚡ Performance Benchmark
- Runtime comparison:
  - Black-Scholes (μs)
  - Monte Carlo Standard (ms)
  - Monte Carlo Antithetic (ms)

### 🧑‍💻 Interactive CLI
- User inputs all parameters dynamically:
  - `S0`, `K`, `r`, `y`, `sigma`, `T`
  - pricing method (Black-Scholes / Monte Carlo)
  - payoff style (European / Asian)
  - option type (Call / Put)
  - simulation count
  - antithetic variates toggle

---

## 🧠 Mathematical Models

### Black-Scholes Formula

\[
C = S_0 e^{-yT} N(d_1) - K e^{-rT} N(d_2)
\]

\[
d_1 = \frac{\ln(S_0/K) + (r - y + 0.5\sigma^2)T}{\sigma \sqrt{T}}
\]

\[
d_2 = d_1 - \sigma \sqrt{T}
\]

---

### Geometric Brownian Motion (GBM)

\[
S_T = S_0 \exp\left((r - y - 0.5\sigma^2)T + \sigma \sqrt{T} Z \right)
\]

---

### Monte Carlo Estimator

\[
V = e^{-rT} \cdot \mathbb{E}[\text{Payoff}]
\]

---

### Antithetic Variates

Use paired samples:
\[
Z \quad \text{and} \quad -Z
\]

To reduce variance in Monte Carlo estimation.

---

## 📁 Project Structure
[
](https://github.com/halim-quantlab/option-pricing-engine-cpp/blob/main/image/structure.png?raw=true)<img width="624" height="794" alt="image" src="https://github.com/user-attachments/assets/050272bc-9f4b-4956-bcfc-158a212821fa" />
---
## Input
[
](https://github.com/halim-quantlab/option-pricing-engine-cpp/blob/main/image/image.png?raw=true)<img width="384" height="358" alt="image" src="https://github.com/user-attachments/assets/8d202f08-80da-41ba-b3d3-4856664e268c" />

---

## Example Output

[
](https://github.com/halim-quantlab/option-pricing-engine-cpp/blob/main/image/output.png?raw=true)<img width="1311" height="735" alt="image" src="https://github.com/user-attachments/assets/064f827f-fcf8-4722-bd1b-9d7aab46650e" />

---
## Compile
g++ -std=c++17 main.cpp src/math_utils.cpp src/black_scholes.cpp src/monte_carlo.cpp src/analysis.cpp -o option_engine
./option_engine

---
## 🎯 Key Learnings
	•	Built a modular C++ derivatives pricing engine
	•	Implemented stochastic simulation with GBM
	•	Applied Monte Carlo variance reduction
	•	Validated numerical methods against analytical pricing
	•	Performed convergence and runtime analysis
	•	Structured the codebase using reusable headers and source files
---

🧩 Model Interpretation

Black-Scholes Assumptions
	- Asset follows Geometric Brownian Motion (GBM)
	- Constant volatility
	- Constant risk-free rate
	- Continuous dividend yield
	- No arbitrage opportunities
	- Frictionless markets (no transaction cost, perfect liquidity)
	- Continuous trading
	- Lognormal return distribution

⸻

When Black-Scholes Breaks
	- Volatility is not constant (volatility smile/skew)
	- Sudden jumps (earnings, macro shocks) violate GBM
	- Interest rates and dividends change over time
	- Transaction costs and liquidity constraints exist
	- Discrete hedging introduces errors
	- Not suitable for path-dependent options like arithmetic Asian

⸻

Monte Carlo Assumptions
	- Underlying follows GBM
	- Random variables are normally distributed
	- Large number of simulations ensures convergence
	- Constant discount rate

⸻

Monte Carlo Limitations
	- Model risk persists (depends on GBM assumption)
	- Slow convergence for high accuracy
	- High computational cost
	- Tail risks may be underrepresented
	- No closed-form benchmark for some exotic options

🚀 Future Improvements

Pricing Models
	- Binomial Tree
	- Trinomial Tree
	- Barrier Options
	- Lookback Options
	- Digital Options

This project is designed as a foundational quantitative finance engine for:
	- Understanding derivatives pricing
	- Comparing analytical vs numerical methods
	- Studying Monte Carlo convergence
	- Building reusable C++ quant infrastructure


👤 Author
Wielly Halim
Aspiring Quant Researcher | C++ & Python | Derivatives Pricing

If you found this project insightful, consider starring the repository.

