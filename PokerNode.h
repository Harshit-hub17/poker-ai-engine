#ifndef POKERNODE_H
#define POKERNODE_H

#include <string>
#include <vector>

struct Player {
    int id;
    std::string name;
    std::vector<std::string> holeCards;
    int chips = 100;
    bool active = true;
    bool folded = false;
};

#endif