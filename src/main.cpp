#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include "../include/PokerNode.h"

class HoldemEngine {
private:
    std::vector<std::string> deck;
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
    std::vector<std::string> suits = {"s", "h", "d", "c"};
    std::vector<std::string> board;
    
    std::string C_RED = "\033[1;31m";    
    std::string C_WHT = "\033[1;37m";  
    std::string C_RST = "\033[0m";     

    void buildDeck() {
        deck.clear();
        for (const auto& r : ranks) {
            for (const auto& s : suits) {
                deck.push_back(r + s);
            }
        }
    }

    std::string getSuitSymbol(char s) {
        if (s == 's') return "♠";
        if (s == 'h') return "♥";
        if (s == 'd') return "♦";
        return "♣";
    }

    void printCardsVisual(const std::vector<std::string>& cardList) {
        std::string l1 = "", l2 = "", l3 = "", l4 = "";
        for (const auto& card : cardList) {
            std::string r = card.substr(0, 1);
            char s_type = card;
            std::string sym = getSuitSymbol(s_type);
            std::string color = (s_type == 'h' || s_type == 'd') ? C_RED : C_WHT;
            
            l1 += color + " ┌───┐ " + C_RST;
            l2 += color + " │" + r + "  │ " + C_RST;
            l3 += color + " │  " + sym + "│ " + C_RST;
            l4 += color + " └───┘ " + C_RST;
        }
        std::cout << l1 << "\n" << l2 << "\n" << l3 << "\n" << l4 << "\n";
    }

    int evaluateDummyStrength(const std::vector<std::string>& hole, const std::vector<std::string>& brd) {
        int highest = 0;
        auto getWeight = [&](char r) {
            if (r == 'A') return 14; if (r == 'K') return 13;
            if (r == 'Q') return 12; if (r == 'J') return 11;
            if (r == 'T') return 10; return r - '0';
        };
        for (const auto& c : hole) highest = std::max(highest, getWeight(c));
        for (const auto& c : brd)  highest = std::max(highest, getWeight(c));
        
        std::map<char, int> counts;
        for (const auto& c : hole) counts[c]++;
        for (const auto& c : brd) counts[c]++;
        for (auto const& [rank, qty] : counts) {
            if (qty >= 2) return 100 + getWeight(rank);
        }
        return highest;
    }

public:
    void runHand() {
        buildDeck();
        
        std::random_device rd; 
        std::mt19935 rng(rd()); 
        
        std::shuffle(deck.begin(), deck.end(), rng);

        std::vector<Player> players = {
            {0, "You (Player 1)", {}, 100, true, false},
            {1, "Bot Alpha", {}, 100, true, false},
            {2, "Bot Bravo", {}, 100, true, false},
            {3, "Bot Charlie", {}, 100, true, false}
        };

        for (int i = 0; i < 2; ++i) {
            for (auto& p : players) {
                p.holeCards.push_back(deck.back());
                deck.pop_back();
            }
        }

        board.clear();
        for (int i = 0; i < 5; ++i) {
            board.push_back(deck.back());
            deck.pop_back();
        }

        std::vector<std::string> flop = {board, board, board};

        std::cout << "\n=============================================\n";
        std::cout << "      4-PLAYER TEXAS HOLD'EM TERMINAL MATCH  \n";
        std::cout << "=============================================\n";
        std::cout << "YOUR PRIVATE POCKET HAND:\n";
        printCardsVisual(players.holeCards);
        
        std::cout << "THE FLOP BOARD:\n";
        printCardsVisual(flop);
        std::cout << "---------------------------------------------\n";

        std::cout << "👉 Action is on you! (Ante: 10 chips)\n";
        std::cout << "Choose option -> (b)et/call or (f)old: ";
        std::string move; std::cin >> move;

        if (move == "f" || move == "fold") {
            std::cout << "\n❌ You folded! The remaining bots complete the round.\n";
            players.folded = true;
        } else {
            std::cout << "\n💰 You place a live bet! Bots are calculating responses...\n";
        }

        std::uniform_real_distribution<double> coinFlip(0.0, 1.0);
        
        for (size_t i = 1; i < players.size(); ++i) {
            int score = evaluateDummyStrength(players[i].holeCards, flop);
            double foldProbability = 0.50; 
            
            if (score >= 100) {
                foldProbability = 0.15; 
            } else if (score > 10) {
                foldProbability = 0.40;
            }

            if (coinFlip(rng) < foldProbability) {
                players[i].folded = true;
                std::cout << "🏳️ " << players[i].name << " folds their hand.\n";
            } else {
                std::cout << "🤖 " << players[i].name << " places a CALL.\n";
            }
        }

        std::cout << "\n=============================================\n";
        std::cout << "                THE SHOWDOWN                 \n";
        std::cout << "=============================================\n";
        std::cout << "COMPLETE COMMUNITY BOARD:\n";
        printCardsVisual(board);

        int bestScore = -1;
        std::string winnerName = "";

        for (auto& p : players) {
            std::cout << "\n" << p.name << " cards:\n";
            printCardsVisual(p.holeCards);
            
            if (p.folded) {
                std::cout << "Status: [Folded]\n";
                continue;
            }

            int finalScore = evaluateDummyStrength(p.holeCards, board);
            std::cout << "Strength Metric Score: " << finalScore << "\n";
            
            if (finalScore > bestScore) {
                bestScore = finalScore;
                winnerName = p.name;
            }
        }

        std::cout << "\n🏆 👑 ROUND WINNER: " << winnerName << " takes the pot!\n";
    }
};

int main() {
    HoldemEngine game;
    game.runHand();
    return 0;
}
