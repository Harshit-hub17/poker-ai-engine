# 🃏 4-Player Texas Hold'em Poker Bot (C++)

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Active-success.svg)]()

A high-performance, probability-driven Texas Hold'em poker engine built entirely in **C++17**. Unlike rigid pattern-based bots, this engine utilizes dynamic game states, multi-player seat rotations, and Monte Carlo simulations to calculate real-time win probabilities and expected values (EV) based on pot odds.

---

## 🏛️ Core Architecture

* **Monte Carlo Simulation Engine:** Evaluates thousands of random game rollouts per turn across a 4-player table to determine precise hand strength and win rates under uncertainty.
* **Probability & Luck-Based Decision Loop:** Dynamically balances pot odds against calculated win probabilities to make mathematically sound decisions (`Fold`, `Check`, `Call`, `Raise`).
* **Pure C++ Backend:** Optimized for speed and low latency, utilizing modern C++ headers and robust memory management without heavy external script dependencies.

---

## 📂 Project Structure

```text
poker-ai-engine/
│
├── PokerNode.h       # Core game state nodes, player structures, and hand evaluation logic
├── main.cpp          # Execution entry point, game loop, and simulation orchestrator
├── requirements.txt  # Environment configuration or build notes
└── README.md         # Project documentation
