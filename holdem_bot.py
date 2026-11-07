import random
from treys import Card, Evaluator

evaluator = Evaluator()

class VisualHoldemGame:
    def __init__(self):
        self.ranks = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A']
        self.suits = ['s', 'h', 'd', 'c']
        # Map symbols for visual layout rendering
        self.suit_symbols = {'s': '♠', 'h': '♥', 'd': '♦', 'c': '♣'}
        
    def render_cards(self, card_strings):
        """Converts raw lists like ['As', 'Kh'] into clean horizontal ASCII blocks."""
        lines = ["", "", "", ""]
        for card in card_strings:
            rank = card[0]
            suit = self.suit_symbols.get(card[1], card[1])
            
            lines[0] +=" ┌───┐ "
            lines[1] +=f" │{rank}  │ "
            lines[2] +=f" │  {suit}│ "
            lines[3] +=" └───┘ "
            
        return "\n".join(lines)

    def run_hand(self, current_chips):
        deck = [f"{r}{s}" for r in self.ranks for s in self.suits]
        random.shuffle(deck)
        
        human_hand_str = [deck.pop(), deck.pop()]
        bot_hand_str = [deck.pop(), deck.pop()]
        
        flop = [deck.pop(), deck.pop(), deck.pop()]
        turn = [deck.pop()]
        river = [deck.pop()]
        
        current_board = flop.copy()
        
        print("\n" + "🃏 " * 15)
        print(f"       CURRENT CHIP BALACE: {current_chips} Chips")
        print("🃏 " * 15)
        print("YOUR PRIVATE POCKET HAND:")
        print(self.render_cards(human_hand_str))
        print(f"THE FLOP BOARD:")
        print(self.render_cards(current_board))
        print("-" * 45)
        
        # --- Interactive Betting Round ---
        print("👉 Action is on you! (Ante cost: 10 chips. Betting adds 20 more)")
        action = input("Choose action - (b)et/call or (f)old: ").strip().lower()
        
        pot = 20  # 10 chips ante from each player
        
        if action.startswith('f'):
            print("\n❌ You folded! The Bot steals the pot.")
            print("Bot's hidden cards were:")
            print(self.render_cards(bot_hand_str))
            return -10 # Lost your ante
            
        elif action.startswith('b'):
            pot += 40 # 20 more chips from each player
            print("\n💰 You put 20 more chips into the pot!")
            
            # Simple AI decision threshold using Treys strength metrics
            bot_cards = [Card.new(c) for c in bot_hand_str]
            board_cards = [Card.new(c) for c in current_board]
            bot_score = evaluator.evaluate(board_cards, bot_cards)
            
            if bot_score < 6000 or random.random() < 0.25: 
                print("🤖 Bot checks its equity calculations... and places a CALL!")
            else:
                print("🏳️ Bot folds! Your bluff worked perfectly. You win the pot!")
                print("Bot folded:")
                print(self.render_cards(bot_hand_str))
                return 10 # Won the bot's ante

        # --- The Showdown ---
        current_board.extend(turn + river)
        print("\n" + "═" * 45)
        print("                THE SHOWDOWN                 ")
        print("═" * 45)
        print("YOUR FINAL HAND:")
        print(self.render_cards(human_hand_str))
        print("BOT'S FINAL HAND:")
        print(self.render_cards(bot_hand_str))
        print("COMPLETE RUNOUT BOARD:")
        print(self.render_cards(current_board))
        print("-" * 45)
        
        # Evaluate combinations via the internal Treys module
        h_hand = [Card.new(c) for c in human_hand_str]
        b_hand = [Card.new(c) for c in bot_hand_str]
        final_board = [Card.new(c) for c in current_board]
        
        h_score = evaluator.evaluate(final_board, h_hand)
        b_score = evaluator.evaluate(final_board, b_hand)
        
        h_name = evaluator.class_to_string(evaluator.get_rank_class(h_score))
        b_name = evaluator.class_to_string(evaluator.get_rank_class(b_score))
        
        print(f"YOUR STRENGTH:  {h_name}")
        print(f"BOT'S STRENGTH: {b_name}")
        print("-" * 45)
        
        if h_score < b_score:
            win_amount = pot // 2
            print(f"🎉 SHOWDOWN VICTORY: You win the pot! (+{win_amount} chips)")
            return win_amount
        elif b_score < h_score:
            loss_amount = pot // 2
            print(f"🤖 SHOWDOWN DEFEAT: The Bot takes the pot! (-{loss_amount} chips)")
            return -loss_amount
        else:
            print("🤝 DRAW: Chopped pot! (Chips returned)")
            return 0

    def start_tournament(self, total_rounds=5):
        chips = 100 # Starting Bankroll
        print("\n" + "═" * 45)
        print(f"  STARTING POKER TOURNAMENT: {total_rounds} HANDS MATCH  ")
        print("═" * 45)
        
        for r in range(1, total_rounds + 1):
            print(f"\n✨ --- STARTING ROUND {r} OF {total_rounds} ---")
            chip_delta = self.run_hand(chips)
            chips += chip_delta
            
            input("\nPress Enter to proceed to the next hand deal...")
            
        print("\n" + "=" * 45)
        print("               TOURNAMENT OVER               ")
        print("=" * 45)
        print(f"Your Final Ending Bankroll: {chips} Chips")
        if chips > 100:
            print("🏆 Status: Congratulations! You outsmarted the AI.")
        elif chips < 100:
            print("📉 Status: The AI extracted maximum value. Better luck next time!")
        else:
            print("🤝 Status: You broke perfectly even against the bot.")

if __name__ == "__main__":
    game = VisualHoldemGame()
    game.start_tournament(total_rounds=5)