import numpy as np
import random

# Game constants
P_FOLD = 0
P_BET = 1
ACT_SIZE = 2

class PokerNode:
    def __init__(self, key):
        self.key = key
        self.regrets = np.zeros(ACT_SIZE)
        self.strat = np.zeros(ACT_SIZE)
        self.strat_sum = np.zeros(ACT_SIZE)

    def compute_strat(self, weight):
        pos_r = np.maximum(self.regrets, 0)
        total = np.sum(pos_r)
        
        if total > 0:
            self.strat = pos_r / total
        else:
            self.strat = np.repeat(1.0 / ACT_SIZE, ACT_SIZE)
            
        self.strat_sum += weight * self.strat
        return self.strat

    def get_final_strat(self):
        total = np.sum(self.strat_sum)
        if total > 0:
            return self.strat_sum / total
        return np.repeat(1.0 / ACT_SIZE, ACT_SIZE)

class SolverCFR:
    def __init__(self):
        self.nodes = {}

    def run_cfr(self, cards, history, p0, p1):
        depth = len(history)
        curr_act = depth % 2
        opp_act = 1 - curr_act

        # Showdown terminal states
        if depth > 1:
            folded = history[-1] == 'p'
            called = history[-2:] == 'bb'
            win = cards[curr_act] > cards[opp_act]
            
            if folded:
                return 1 if history == 'pp' and win else (1 if history != 'pp' else -1)
            if called:
                return 2 if win else -2

        state_key = f"{cards[curr_act]}_{history}"
        if state_key not in self.nodes:
            self.nodes[state_key] = PokerNode(state_key)
        n = self.nodes[state_key]

        w = p0 if curr_act == 0 else p1
        curr_strat = n.compute_strat(w)
        utils = np.zeros(ACT_SIZE)
        expected_val = 0

        for a in range(ACT_SIZE):
            next_h = history + ('p' if a == P_FOLD else 'b')
            if curr_act == 0:
                utils[a] = -self.run_cfr(cards, next_h, p0 * curr_strat[a], p1)
            else:
                utils[a] = -self.run_cfr(cards, next_h, p0, p1 * curr_strat[a])
            expected_val += curr_strat[a] * utils[a]

        for a in range(ACT_SIZE):
            r = utils[a] - expected_val
            n.regrets[a] += (p1 if curr_act == 0 else p0) * r

        return expected_val

    def start_training(self, loops):
        deck = [1, 2, 3] # J, Q, K
        for _ in range(loops):
            random.shuffle(deck)
            self.run_cfr(deck, "", 1.0, 1.0)

def start_match(ai):
    names = {1: "Jack", 2: "Queen", 3: "King"}
    print("\n--- KUHN POKER (TEST ENGINE v1.0) ---")
    
    while True:
        deck = [1, 2, 3]
        random.shuffle(deck)
        p_card, bot_card = deck[0], deck[1]
        p1_acts = random.choice([True, False])
        h = ""
        
        print(f"\n[Hand] Dealt: {names[p_card]}")
        
        if p1_acts:
            print(">> Action is on you first.")
            act = input("Action? (p = check, b = bet): ").strip().lower()
            h += 'p' if act == 'p' else 'b'
            
            key = f"{bot_card}_{h}"
            bot_s = ai.nodes[key].get_final_strat()
            bot_a = 'p' if random.random() < bot_s[0] else 'b'
            print(f"Bot action: {bot_a}")
            h += bot_a
            
            if h == 'pb':
                act = input("Bot raised. (p = fold, b = call): ").strip().lower()
                h += 'p' if act == 'p' else 'b'
        else:
            key = f"{bot_card}_"
            bot_s = ai.nodes[key].get_final_strat()
            bot_a = 'p' if random.random() < bot_s[0] else 'b'
            print(f"Bot acts first: {bot_a}")
            h += bot_a
            
            act = input("Your turn (p = check/fold, b = bet/call): ").strip().lower()
            h += 'p' if act == 'p' else 'b'
            
            if h == 'pb':
                key = f"{bot_card}_{h}"
                bot_s = ai.nodes[key].get_final_strat()
                bot_a = 'p' if random.random() < bot_s[0] else 'b'
                print(f"Bot response to bet: {bot_a}")
                h += bot_a

        print(f"Showdown -> Bot held: {names[bot_card]}")
        good = p_card > bot_card
        
        if h in ['pp', 'bb']:
            m = 1 if h == 'pp' else 2
            print("You Win!" if good else "Bot Wins!")
        elif h in ['bp', 'pbp']:
            # Checked fold scenarios
            if (p1_acts and h == 'bp') or (not p1_acts and h == 'pbp'):
                print("Bot folded. You win.")
            else:
                print("You folded. Bot wins.")
                
        if input("\nNext hand? (y/n): ").strip().lower() != 'y':
            break

if __name__ == "__main__":
    print("Running background CFR training arrays...")
    brain = SolverCFR()
    brain.start_training(100000)
    start_match(brain)