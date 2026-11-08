# 4-Player Texas Hold'em Poker Bot (C++)

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue.svg) ![License](https://img.shields.io/badge/License-MIT-yellow.svg) ![Status](https://img.shields.io/badge/Status-Active-success.svg)

A high-performance, probability-driven Texas Hold'em poker engine built in C++. This engine utilizes dynamic game states, multi-player seat rotations, and core game logic to simulate and calculate hand strength and decisions based on pot odds.

## 🏛️ Core Architecture

* **Game State Management (`PokerNode.h`):** Structures player states, chips, active turns, and hand evaluation logic cleanly encapsulated in header files.
* **Probability & Decision Loop:** Dynamically evaluates game rules and player actions (`Fold`, `Check`, `Call`, `Raise`) during terminal-based execution.
* **Pure C++ Backend:** Optimized for speed and low latency using standard C++ structures and headers without heavy external dependencies.

## 📂 Project Structure

```text
poker-ai-engine/
├── include/
│   └── PokerNode.h     # Core game state nodes and player structures
├── src/
│   └── main.cpp        # Execution entry point and game loop
└── README.md           # Project documentation
