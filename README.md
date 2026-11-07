# 🃏 4-Player Texas Hold'em Poker Bot (C++)

A high-performance, probability-driven Texas Hold'em poker engine built entirely in **C++17**. Unlike rigid pattern-based bots, this engine utilizes dynamic game states, multi-player seat rotations, and **Monte Carlo simulations** to calculate real-time win probabilities and expected values (EV) based on pot odds.

## 🚀 Core Architecture
* **Monte Carlo Simulation Engine:** Evaluates thousands of random game rollouts per turn across a 4-player table to determine precise hand strength.
* **Probability & Luck-Based Decision Loop:** Dynamically balances pot odds against win probabilities to make mathematically sound decisions (Fold, Check, Call, Raise).
* **Pure C++ Backend:** Optimized for speed, utilizing modern C++ headers and robust memory management without external script dependencies.

## 🛠️ Build & Run Instructions
## 🛠️ Build & Run Instructions
1. Clone the repository:
   ```bash
   git clone [https://github.com/Harshit-hub17/poker-ai-engine.git](https://github.com/Harshit-hub17/poker-ai-engine.git)
   cd poker-ai-engine
