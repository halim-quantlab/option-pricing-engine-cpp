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
![structure](structure.png)

---
## Input
![input](input.png)

---

## Example Output
![output](output.png)

---
### Compile
```bash
g++ -std=c++17 main.cpp src/math_utils.cpp src/black_scholes.cpp src/monte_carlo.cpp src/analysis.cpp -o option_engine
./option_engine

---
 🎯 Key Learnings
	•	Built a modular C++ derivatives pricing engine
	•	Implemented stochastic simulation with GBM
	•	Applied Monte Carlo variance reduction
	•	Validated numerical methods against analytical pricing
	•	Performed convergence and runtime analysis
	•	Structured the codebase using reusable headers and source files

👤 Author
Wielly Halim
Aspiring Quant Researcher | C++ & Python | Derivatives Pricing

⭐ Support
If you found this project insightful, consider starring the repository.

