import random
from treys import Card, Evaluator

# Init check wrapper
poker_eval = Evaluator()

class TexasGame:
    def __init__(self):
        self.ranks = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A']
        self.suits = ['s', 'h', 'd', 'c']
        self.symbols = {'s': '♠', 'h': '♥', 'd': '♦', 'c': '♣'}
        
        # UI color constants
        self.C_RED = "\033[1;31m"    
        self.C_WHT = "\033[1;37m"  
        self.C_RST = "\033[0m"     

    def display_hand(self, card_list):
        """Standard visual grid layout for text interfaces."""
        l1, l2, l3, l4 = "", "", "", ""
        for c in card_list:
            r = c[0]
            s_type = c[1]
            sym = self.symbols.get(s_type, s_type)
            color = self.C_RED if s_type in ['h', 'd'] else self.C_WHT
            
            l1 += f"{color} ┌───┐ {self.C_RST}"
            l2 += f"{color} │{r}  │ {self.C_RST}"
            l3 += f"{color} │  {sym}│ {self.C_RST}"
            l4 += f"{color} └───┘ {self.C_RST}"
            
        return f"{l1}\n{l2}\n{l3}\n{l4}"

    def play_round(self, bankroll):
        # Build 52 card array loop
        deck = [f"{r}{s}" for r in self.ranks for s in self.suits]
        random.shuffle(deck)
        
        p_hand = [deck.pop(), deck.pop()]
        b_hand = [deck.pop(), deck.pop()]
        
        flop = [deck.pop(), deck.pop(), deck.pop()]
        turn = [deck.pop()]
        river = [deck.pop()]
        
        board = flop.copy()
        
        print("\n" + "==" * 20)
        print(f" BANKROLL STATUS: {bankroll} CHIPS")
        print("==" * 20)
        print("YOUR HAND:")
        print(self.display_hand(p_hand))
        print("FLOP CARDS:")
        print(self.display_hand(board))
        print("-" * 40)
        
        # Interactive prompt check
        act = input("Your Move -> (b)et [20 chips] or (f)old: ").strip().lower()
        pot = 20 # Ante base setup
        
        if act.startswith('f'):
            print("\n[!] You folded. Bot takes it.")
            print("Bot cards were:")
            print(self.display_hand(b_hand))
            return -10
            
        elif act.startswith('b'):
            pot += 40
            
            # Simple AI threshold mapping logic
            b_cards = [Card.new(c) for c in b_hand]
            brd_cards = [Card.new(c) for c in board]
            b_score = poker_eval.evaluate(brd_cards, b_cards)
            
            # TODO: implement dynamic sizing calculation here later
            if b_score < 6000 or random.random() < 0.20: 
                print("🤖 Bot calls your bet.")
            else:
                print("🏳️ Bot folds! Hand won via bluff.")
                print("Bot mucked:")
                print(self.display_hand(b_hand))
                return 10

        # Run out the rest of the board cards
        board.extend(turn + river)
        print("\n" + " Showdown Runout ".center(40, "="))
        print("YOUR HAND:")
        print(self.display_hand(p_hand))
        print("BOT HAND:")
        print(self.display_hand(b_hand))
        print("FINAL BOARD:")
        print(self.display_hand(board))
        print("-" * 40)
        
        # Treys module engine valuation
        h_cards = [Card.new(c) for c in p_hand]
        bot_cards = [Card.new(c) for c in b_hand]
        final_brd = [Card.new(c) for c in board]
        
        p_score = poker_eval.evaluate(final_brd, h_cards)
        bot_score = poker_eval.evaluate(final_brd, bot_cards)
        
        p_class = poker_eval.get_rank_class(p_score)
        b_class = poker_eval.get_rank_class(bot_score)
        
        print(f"Your combination:  {poker_eval.class_to_string(p_class)}")
        print(f"Bot combination:   {poker_eval.class_to_string(b_class)}")
        print("-" * 40)
        
        if p_score < bot_score:
            print(f"Victory! Collected pot of {pot // 2} chips.")
            return pot // 2
        elif bot_score < p_score:
            print(f"Defeat. Lost {pot // 2} chips.")
            return -(pot // 2)
        else:
            print("Split pot balance returned.")
            return 0

    def start_loop(self, total_hands=5):
        stack = 100
        print(f"--- Running {total_hands}-Hand Session ---")
        
        for idx in range(1, total_hands + 1):
            print(f"\n[Hand #{idx}] Dealing cards...")
            change = self.play_round(stack)
            stack += change
            
            input("\nPress Enter for next deal...")
            
        print("\n" + " Session Stats ".center(40, "="))
        print(f"Ending balance: {stack} chips")
        if stack > 100:
            print("Session Result: Profit")
        elif stack < 100:
            print("Session Result: Loss")
        else:
            print("Session Result: Even")

if __name__ == "__main__":
    engine = TexasGame()
    engine.start_loop(5)