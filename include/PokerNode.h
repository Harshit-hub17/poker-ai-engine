#ifndef POKERNODE_H
#define POKERNODE_H

#include <string>
#include <vector>

struct Player {
    int id;
    std::string name;
    std::vector<std::string> holeCards;
    int chips = 1000;
    int currentBet = 0;
    int totalHandBet = 0;
    bool active = true;
    bool folded = false;
    bool isAllIn = false;
    bool isEliminated = false;
};

#endif
