#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include "PokerNode.h"

class HoldemEngine {
private:
    std::vector<std::string> deck;
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
    std::vector<std::string> suits = {"s", "h", "d", "c"};
    std::vector<std::string> board;
    
    std::vector<Player> players;
    
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
            char s_type = card[1];
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
    std::vector<std::string> sevenCards = brd;
    sevenCards.push_back(hole[0]);
    sevenCards.push_back(hole[1]);

    auto getRank = [](const std::string& card) {
        
        if (card.rfind("10", 0) == 0) return 10;
        
        char r = card[0];
        if (r == 'A') return 14;
        if (r == 'K') return 13;
        if (r == 'Q') return 12;
        if (r == 'J') return 11;
        if (r == 'T') return 10;
        return r - '0';
    };

    auto getSuit = [](const std::string& card) {
        return card.back();
    };

    std::map<int, int> rankCounts;
    std::map<char, int> suitCounts;

    for (const auto& card : sevenCards) {
        rankCounts[getRank(card)]++;
        suitCounts[getSuit(card)]++;
    }

    bool hasFlush = false;
    for (const auto& pair : suitCounts) {
        if (pair.second >= 5) { hasFlush = true; break; }
    }

    std::vector<int> uniqueRanks;
    for (const auto& pair : rankCounts) {
        uniqueRanks.push_back(pair.first);
    }
    
    if (rankCounts.count(14)) uniqueRanks.push_back(1);
    std::sort(uniqueRanks.begin(), uniqueRanks.end());

    bool hasStraight = false;
    int straightHigh = 0;
    int streak = 1;
    for (size_t i = 1; i < uniqueRanks.size(); ++i) {
        if (uniqueRanks[i] == uniqueRanks[i - 1] + 1) {
            streak++;
            if (streak >= 5) {
                hasStraight = true;
                straightHigh = uniqueRanks[i];
            }
        } else if (uniqueRanks[i] != uniqueRanks[i - 1]) {
            streak = 1;
        }
    }

    int pairs = 0;
    bool threeOfAKind = false;
    bool fourOfAKind = false;
    int highCard = 0;

    for (const auto& pair : rankCounts) {
        if (pair.second == 4) fourOfAKind = true;
        if (pair.second == 3) threeOfAKind = true;
        if (pair.second == 2) pairs++;
        if (pair.first > highCard) highCard = pair.first;
    }

    if (fourOfAKind) return 700 + highCard;
    if (threeOfAKind && pairs >= 1) return 600 + highCard; 
    if (hasFlush) return 500 + highCard;
    if (hasStraight) return 400 + straightHigh;           
    if (threeOfAKind) return 300 + highCard;
    if (pairs >= 2) return 200 + highCard;
    if (pairs == 1) return 100 + highCard;

    return highCard;
}

public:
void handleBettingRound(int& pot, int& highestBet) {
    bool roundComplete = false;

    while (!roundComplete) {
        roundComplete = true;

        for (auto& p : players) {
            if (p.folded ||p.isAllIn || p.isEliminated || p.chips == 0) continue;

            int amountToCall = highestBet - p.currentBet;

            if (p.name == "You (Player 1)") {
                std::cout << "\n----------------------------------------\n";
                std::cout << "YOURPRIVATE POCKET HAND:\n";
                printCardsVisual(p.holeCards);
                
                std::cout <<"\n------------------------------------------\n";
                std::cout << " Pot: $" << pot << " | Highest Bet: $" << highestBet << " | Your Chips: $" << p.chips << "\n";
                std::cout << " Your current round contribution: $" << p.currentBet << "\n";
                std::cout << "Choose action -> (c)heck/call, (r)aise, (f)old, (a)ll-in: ";
                char choice;
                std::cin >> choice;

                if (choice == 'f') {
                    p.folded = true;
                    std::cout << "You folded your hand.\n";
                } 
                else if (choice == 'r') {
                    int raiseAmount;
                    std::cout << "Enter total bet amount you want to match/raise to: ";
                    std::cin >> raiseAmount;
                    int diff = raiseAmount - p.currentBet;
                    p.chips -= diff;
                    p.currentBet = raiseAmount;
                    pot += diff;
                    if (raiseAmount > highestBet) {
                        highestBet = raiseAmount;
                        roundComplete = false;
                    }
                } else if (choice == 'a') {
    int allInAmount = p.chips;
    p.chips = 0;
    p.currentBet += allInAmount;
    p.totalHandBet += allInAmount;
    pot += allInAmount;
    p.isAllIn = true;
    
    if (p.currentBet > highestBet) {
        highestBet = p.currentBet;
        roundComplete = false;
    }
    std::cout << "You shoved ALL-IN for $" << allInAmount << "!\n";
}
                else { 
                    p.chips -= amountToCall;
                    p.currentBet += amountToCall;
                    pot += amountToCall;
                    std::cout << "You call $" << amountToCall << ".\n";
                }
            } 
            else {
                if (amountToCall == 0) {
                    std::cout << p.name << " checks.\n";
                } else if (p.chips >= amountToCall) {
                    p.chips -= amountToCall;
                    p.currentBet += amountToCall;
                    pot += amountToCall;
                    std::cout << p.name << " calls $" << amountToCall << ".\n";
                } else {
                    p.folded = true;
                    std::cout << p.name << " folds.\n";
                }
            }
        }
    }

    for (auto& p : players) {
        p.currentBet = 0;
    }
}
    void runHand() {
        
        board.clear();
        for(auto& p : players) {
            p.holeCards.clear();
            p.folded = false;
            p.currentBet = 0;
        }
        
        buildDeck();
        
        std::random_device rd; 
        std::mt19937 rng(rd()); 
        
        std::shuffle(deck.begin(), deck.end(), rng);


Player p0; p0.id = 0; p0.name = "You (Player 1)"; p0.chips = 100; p0.active = true; p0.folded = false;
players.push_back(p0);

Player p1; p1.id = 1; p1.name = "Alpha"; p1.chips = 100; p1.active = true; p1.folded = false;
players.push_back(p1);

Player p2; p2.id = 2; p2.name = "Bravo"; p2.chips = 100; p2.active = true; p2.folded = false;
players.push_back(p2);

Player p3; p3.id = 3; p3.name = "Charlie"; p3.chips = 100; p3.active = true; p3.folded = false;
players.push_back(p3);

        for (int i = 0; i < 2; ++i) {
            for (auto& p : players) {
                p.holeCards.push_back(deck.back());
                deck.pop_back();
            }
        }

        board.clear();
        for (int i = 0; i < 3; ++i) {
            board.push_back(deck.back());
            deck.pop_back();
            
        }
    int pot = 0;
    int highestBet = 20; 
    
    std::cout << "\n================ PRE-FLOP ================\n";
    handleBettingRound(pot, highestBet);

    board.clear();
    for (int i = 0; i < 3; ++i) {
        board.push_back(deck.back());
        deck.pop_back();
    }
    std::cout << "\n================ THE FLOP ================\n";
    printCardsVisual(board);
    
    highestBet = 0;
    handleBettingRound(pot, highestBet);

    board.push_back(deck.back());
    deck.pop_back();
    std::cout << "\n================ THE TURN ================\n";
    printCardsVisual(board);
    
    highestBet = 0;
    handleBettingRound(pot, highestBet);

    board.push_back(deck.back());
    deck.pop_back();
    std::cout << "\n================ THE RIVER ================\n";
    printCardsVisual(board);
    
    highestBet = 0;
    handleBettingRound(pot, highestBet);
    
    std::cout << "\n================ SHOWDOWN ================\n";


        std::cout << "\n=============================================\n";
        std::cout << "      4-PLAYER TEXAS HOLD'EM TERMINAL MATCH  \n";
        std::cout << "=============================================\n";
        std::cout << "YOUR PRIVATE POCKET HAND:\n";
        printCardsVisual(players[0].holeCards);
        
        std::cout << "THE FLOP BOARD:\n";
        printCardsVisual(board);
        std::cout << "---------------------------------------------\n";

        


        std::uniform_real_distribution<double> coinFlip(0.0, 1.0);
        
       for (size_t i = 1; i < players.size(); ++i) {
    if (players[i].folded) continue;

    bool isLatePos = (i >= players.size() / 2);

    // Call your advanced Monte Carlo & EV decision engine
    std::string decision = getBotDecision(
        players[i].holeCards, 
        board, 
        players.size(), 
        pot, 
        highestBet - players[i].currentBet, 
        players[i].chips, 
        isLatePos
    );

    
    if (decision == "FOLD") {
        players[i].folded = true;
        std::cout << players[i].name << " folds their hand.\n";
    } 
    else if (decision == "CHECK" || decision == "CALL") {
        int callAmount = highestBet - players[i].currentBet;
        int actualBet = std::min(players[i].chips, callAmount);
        players[i].chips -= actualBet;
        players[i].currentBet += actualBet;
        pot += actualBet;
        std::cout << players[i].name << " places a CALL of $" << actualBet << ".\n";
    } 
    else if (decision.rfind("RAISE", 0) == 0) {
        int raiseAmt = std::stoi(decision.substr(6));
        int totalToPutIn = (highestBet - players[i].currentBet) + raiseAmt;
        totalToPutIn = std::min(players[i].chips, totalToPutIn);

        players[i].chips -= totalToPutIn;
        players[i].currentBet += totalToPutIn;
        highestBet = players[i].currentBet;
        pot += totalToPutIn;

        std::cout << players[i].name << " raises by $" << raiseAmt << "!\n";
    }
}

        std::cout << "\n=============================================\n";
        std::cout << "                THE SHOWDOWN                 \n";
        std::cout << "=============================================\n";
        std::cout << "COMPLETE COMMUNITY BOARD:\n";
       
       board.push_back(deck.back());
        deck.pop_back();
        
        board.push_back(deck.back());
        deck.pop_back();
       
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
            std::cout << "Hand: ";
if (finalScore >= 700) std::cout << "Four of a Kind\n";
else if (finalScore >= 600) std::cout << "Full House\n";
else if (finalScore >= 500) std::cout << "Flush\n";
else if (finalScore >= 400) std::cout << "Straight\n";
else if (finalScore >= 300) std::cout << "Three of a Kind\n";
else if (finalScore >= 200) std::cout << "Two Pair\n";
else if (finalScore >= 100) std::cout << "One Pair\n";
else std::cout << "High Card\n";
            
            if (finalScore > bestScore) {
                bestScore = finalScore;
                winnerName = p.name;
            }
        }

        std::cout << "\n🏆 👑 ROUND WINNER: " << winnerName << " takes the pot!\n";
    }
    double runMonteCarlo(const std::vector<std::string>& myHoleCards, const std::vector<std::string>& currentBoard, int totalPlayers, int simulations = 2000) {
        int wins = 0;
        int ties = 0;

        std::random_device rd;
        std::mt19937 rng(rd());

        for (int i = 0; i < simulations; ++i) {
            std::vector<std::string> simDeck = deck; 
            
            auto removeCard = [&](const std::string& c) {
                simDeck.erase(std::remove(simDeck.begin(), simDeck.end(), c), simDeck.end());
            };

            for (const auto& c : myHoleCards) removeCard(c);
            for (const auto& c : currentBoard) removeCard(c);

            std::shuffle(simDeck.begin(), simDeck.end(), rng);

            std::vector<std::vector<std::string>> opponentHands(totalPlayers - 1);
            for (int p = 0; p < totalPlayers - 1; ++p) {
                opponentHands[p].push_back(simDeck.back()); simDeck.pop_back();
                opponentHands[p].push_back(simDeck.back()); simDeck.pop_back();
            }

            std::vector<std::string> simBoard = currentBoard;
            while (simBoard.size() < 5) {
                simBoard.push_back(simDeck.back());
                simDeck.pop_back();
            }

            int myScore = evaluateDummyStrength(myHoleCards, simBoard);
            int bestOpponentScore = -1;
            bool isTie = false;

            for (int p = 0; p < totalPlayers - 1; ++p) {
                int oppScore = evaluateDummyStrength(opponentHands[p], simBoard);
                if (oppScore > bestOpponentScore) {
                    bestOpponentScore = oppScore;
                    isTie = false;
                } else if (oppScore == bestOpponentScore) {
                    isTie = true;
                }
            }

            if (myScore > bestOpponentScore) {
                wins++;
            } else if (myScore == bestOpponentScore && !isTie) {
                ties++;
            }
        }

        return static_cast<double>(wins + (ties / 2.0)) / simulations;
    }
 
    std::string getBotDecision(const std::vector<std::string>& myHoleCards, 
                              const std::vector<std::string>& currentBoard, 
                              int totalPlayers, 
                              int currentPot, 
                              int callAmount, 
                              int botChips, 
                              bool isLatePosition) {
        
       
        double equity = runMonteCarlo(myHoleCards, currentBoard, totalPlayers, 1500);

       
        double potOdds = (callAmount == 0) ? 0.0 : (static_cast<double>(callAmount) / (currentPot + callAmount));
        double ev = (equity * currentPot) - ((1.0 - equity) * callAmount);

       
        double requiredEquity = isLatePosition ? 0.45 : 0.55;

       
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        double roll = dist(rng);

        

       
        if (equity >= requiredEquity + 0.15) {
            int raiseAmt = std::min(botChips, std::max(callAmount * 2, currentPot / 2));
            return "RAISE " + std::to_string(raiseAmt);
        }

      
        if (equity >= requiredEquity) {
            if (roll < 0.35 && callAmount < botChips / 3) {
                int raiseAmt = std::min(botChips, currentPot / 3);
                return "RAISE " + std::to_string(raiseAmt);
            }
            return (callAmount == 0) ? "CHECK" : "CALL";
        }

       
        if (ev > 0 || equity > potOdds) {
            return (callAmount == 0) ? "CHECK" : "CALL";
        }

       
        if (isLatePosition && roll < 0.10 && callAmount <= currentPot / 4 && callAmount > 0) {
            int bluffAmt = std::min(botChips, currentPot / 2);
            return "RAISE " + std::to_string(bluffAmt);
        }

       
        return (callAmount == 0) ? "CHECK" : "FOLD";
    }

};


int main() {
    HoldemEngine game;
    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {
        game.runHand();
        
        std::cout << "\n----------------------------------------\n";
        std::cout << "Play another round? (y/n): ";
        std::cin >> playAgain;
        std::cout << "----------------------------------------\n\n";
    }

    std::cout << "Thanks for playing! Exiting game.\n";
    return 0;
}
