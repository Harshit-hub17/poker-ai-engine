# 🃏 Imperfect Information Poker AI Engine

An interactive, game-theoretic artificial intelligence suite built in Python. The project explores algorithmic optimization through two frameworks: a foundational self-play **Counterfactual Regret Minimization (CFR)** training model for Nash Equilibrium optimization, alongside an advanced **2-card Texas Hold'em simulation engine** featuring full 7-card combination metrics and interactive ASCII rendering.

---

## 🚀 Core Architectures

### 1. The CFR Optimization Brain (`bot.py`)
Traditional AI engines (like Chess) rely on perfect information where all pieces are visible. Because poker features hidden cards, this model utilizes reinforcement learning via self-play:
- Runs **100,000 algorithmic cycles** of playing against copies of itself.
- Computes **Regret Matching** tracking to dynamically shift action probabilities.
- Reaches a stable **Nash Equilibrium** matrix where it automatically discovers advanced play styles (e.g., optimal bluffing frequencies and trap checking) purely from mathematical feedback.

### 2. The Interactive Texas Hold'em Engine (`holdem_bot.py`)
Scales baseline theories up into authentic poker environments:
- Integrates the advanced **Treys evaluation module** to analyze real 7-card combinations (Pairs, Flushes, Straights).
- Features custom **ASCII Card Rendering UI** to paint standard card decks and live suit symbols (`♠`, `♥`, `♦`, `♣`) into the interface.
- Implements an interactive **Betting Round Loop** tracking user decisions, bluff reactions, and active tournament bankroll states across a multi-round game structure.

---

## 💻 Installation & Local Usage

### 1. Clone the project or download the directory files.
### 2. Install the vector math and evaluation dependencies:
```bash
pip install -r requirements.txt
```
### 3. Launch the 5-Round Interactive Tournament Match:
```bash
python holdem_bot.py
```