# 4-Player Texas Hold'em Poker Bot (C++)

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Status](https://img.shields.io/badge/Status-Active-brightgreen.svg)

A high-performance, probability-driven Texas Hold'em poker simulation engine built in modern C++. The project combines an interactive 4-player terminal application with an integrated Monte Carlo simulation engine and Expected Value (EV) decision-making logic for AI opponents.

---

## Key Features

- **Monte Carlo AI Decision Engine:** Replaces static rule-based or random AI with a real-time equity evaluation system running Monte Carlo hand simulations.
- **Expected Value (EV) Calculation:** AI opponents evaluate pot odds, effective stack sizes, and positional advantage to dynamically decide between `FOLD`, `CHECK`, `CALL`, and `RAISE`.
- **Full Betting Round Engine:** Handles complete multi-player betting rounds across all streets: **Pre-Flop**, **Flop**, **Turn**, and **River**.
- **Clean Architecture:** Zero heavy external dependencies—built purely with standard C++ headers and high-performance algorithms.

---

## 🏛️ Core Architecture

- **Game State Management (`include/PokerNode.h`):** Structures player states, chip stacks, betting contributions, and visual hand representations cleanly in modern C++ headers.
- **Monte Carlo Engine (`src/main.cpp`):** Evaluates equity by simulating probabilistic runouts against opponent ranges to compute optimal decision trees.
- **Pure C++ Performance:** Optimized execution speed allowing hundreds of thousands of Monte Carlo iterations per decision in milliseconds.

---

## 📂 Project Structure

```text
poker-ai-engine/
├── include/
│   └── PokerNode.h       # Core game state nodes and player structures
├── src/
│   └── main.cpp          # Main game loop, betting driver, and EV engine
└── README.md             # Project documentation
