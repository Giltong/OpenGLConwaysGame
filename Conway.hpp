//
// Created by GPowers on 11/7/2022.
//

#ifndef CONWAYSGAMEOFLIFE_CONWAY_HPP
#define CONWAYSGAMEOFLIFE_CONWAY_HPP

#include <unordered_set>
#include "vector"
#include "map"
#include "set"

class Conway {
public:
    void tick();
    std::set<std::pair<int,int>> & get_table();
    void set(int x, int y, bool alive);
    void reset();
public:
    int maxPop = 3;
    int minPop = 2;
    int reproductionPop = 3;
private:
    void tickCell(int x, int y);
private:
    std::set<std::pair<int,int>> alive_cells{};
    std::set<std::pair<int,int>> neighbors{};
};


#endif //CONWAYSGAMEOFLIFE_CONWAY_HPP
