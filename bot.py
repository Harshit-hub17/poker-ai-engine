import numpy as np
import random

# Define actions: 0 = Pass/Fold/Check, 1 = Bet/Call
PASS = 0
BET = 1
NUM_ACTIONS = 2

class KuhnNode:
    def __init__(self, info_set):
        self.info_set = info_set
        self.regret_sum = np.zeros(NUM_ACTIONS)
        self.strategy = np.zeros(NUM_ACTIONS)
        self.strategy_sum = np.zeros(NUM_ACTIONS)

    def get_strategy(self, realization_weight):
        pos_regret = np.maximum(self.regret_sum, 0)
        sum_pos_regret = np.sum(pos_regret)
        if sum_pos_regret > 0:
            self.strategy = pos_regret / sum_pos_regret
        else:
            self.strategy = np.repeat(1.0 / NUM_ACTIONS, NUM_ACTIONS)
        self.strategy_sum += realization_weight * self.strategy
        return self.strategy

    def get_average_strategy(self):
        sum_strategy_sum = np.sum(self.strategy_sum)
        if sum_strategy_sum > 0:
            return self.strategy_sum / sum_strategy_sum
        else:
            return np.repeat(1.0 / NUM_ACTIONS, NUM_ACTIONS)

class KuhnCFRTrainer:
    def __init__(self):
        self.node_map = {}

    def cfr(self, cards, history, p0, p1):
        plays = len(history)
        player = plays % 2
        opponent = 1 - player

        if plays > 1:
            terminal_pass = history[-1] == 'p'
            double_bet = history[-2:] == 'bb'
            is_player_card_higher = cards[player] > cards[opponent]
            if terminal_pass:
                if history == 'pp':
                    return 1 if is_player_card_higher else -1
                else:
                    return 1
            elif double_bet:
                return 2 if is_player_card_higher else -2

        info_set = str(cards[player]) + "_" + history
        if info_set not in self.node_map:
            self.node_map[info_set] = KuhnNode(info_set)
        node = self.node_map[info_set]

        strategy = node.get_strategy(p0 if player == 0 else p1)
        util = np.zeros(NUM_ACTIONS)
        node_util = 0

        for a in range(NUM_ACTIONS):
            next_history = history + ('p' if a == PASS else 'b')
            if player == 0:
                util[a] = -self.cfr(cards, next_history, p0 * strategy[a], p1)
            else:
                util[a] = -self.cfr(cards, next_history, p0, p1 * strategy[a])
            node_util += strategy[a] * util[a]

        for a in range(NUM_ACTIONS):
            regret = util[a] - node_util
            node.regret_sum[a] += (p1 if player == 0 else p0) * regret

        return node_util

    def train(self, iterations):
        cards = [1, 2, 3]
        for _ in range(iterations):
            random.shuffle(cards)
            self.cfr(cards, "", 1.0, 1.0)

# --- Interactive Game Loop ---
def play_game(trainer):
    card_names = {1: "Jack", 2: "Queen", 3: "King"}
    print("\n" + "="*40)
    print("  WELCOME TO KUHN POKER: YOU VS BOT  ")
    print("="*40)
    print("Rules: Deck has Jack, Queen, King. 1 chip ante.")
    
    while True:
        cards = [1, 2, 3]
        random.shuffle(cards)
        human_card, bot_card = cards[0], cards[1]
        
        # Randomly choose who goes first
        human_turn = random.choice([True, False])
        history = ""
        
        print(f"\nYou are dealt: {card_names[human_card]}")
        
        if human_turn:
            print("You are Player 1 (First to act).")
            # Human Action 1
            move = input("Choose action - (p)ass/check or (b)et: ").strip().lower()
            history += 'p' if move.startswith('p') else 'b'
            
            # Bot Action 1
            info_set = f"{bot_card}_{history}"
            bot_strat = trainer.node_map[info_set].get_average_strategy()
            bot_move = 'p' if random.random() < bot_strat[0] else 'b'
            print(f"Bot sees history '{history}' and chooses to: {'Pass/Fold' if bot_move == 'p' else 'Bet/Call'}")
            history += bot_move
            
            # Human Action 2 (if applicable)
            if history == 'pb':
                move = input("Bot bet! Choose action - (p)ass/fold or (b)et/call: ").strip().lower()
                history += 'p' if move.startswith('p') else 'b'
        else:
            print("Bot is Player 1 (First to act).")
            # Bot Action 1
            info_set = f"{bot_card}_"
            bot_strat = trainer.node_map[info_set].get_average_strategy()
            bot_move = 'p' if random.random() < bot_strat[0] else 'b'
            print(f"Bot chooses to: {'Pass/Check' if bot_move == 'p' else 'Bet'}")
            history += bot_move
            
            # Human Action 1
            move = input(f"Bot did '{bot_move}'. Choose action - (p)ass or (b)et: ").strip().lower()
            history += 'p' if move.startswith('p') else 'b'
            
            # Bot Action 2 (if applicable)
            if history == 'pb':
                info_set = f"{bot_card}_{history}"
                bot_strat = trainer.node_map[info_set].get_average_strategy()
                bot_move = 'p' if random.random() < bot_strat[0] else 'b'
                print(f"Bot faces your bet and chooses to: {'Pass/Fold' if bot_move == 'p' else 'Bet/Call'}")
                history += bot_move

        # Evaluate showdown
        print(f"\nShowdown! Bot held: {card_names[bot_card]}")
        is_human_higher = human_card > bot_card
        
        if history in ['pp', 'bb']:
            multiplier = 1 if history == 'pp' else 2
            if is_human_higher:
                print(f"Your card is higher! You win {multiplier} chip(s).")
            else:
                print(f"Bot's card is higher! Bot wins {multiplier} chip(s).")
        elif history == 'bp' or history == 'pbp':
            if (human_turn and history == 'bp') or (not human_turn and history == 'pbp'):
                print("Bot folded! You win 1 chip.")
            else:
                print("You folded! Bot wins 1 chip.")
                
        if input("\nPlay another hand? (y/n): ").strip().lower() != 'y':
            break

if __name__ == "__main__":
    print("Training bot over 100,000 iterations to achieve GTO stability...")
    trainer = KuhnCFRTrainer()
    trainer.train(100000)
    play_game(trainer)