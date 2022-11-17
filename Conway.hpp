//
// Created by GPowers on 11/7/2022.
//

#ifndef CONWAYSGAMEOFLIFE_CONWAY_HPP
#define CONWAYSGAMEOFLIFE_CONWAY_HPP

#include "vector"

class Conway {
public:
    Conway(int max_size);
    void tick();
    std::vector<std::pair<int, int>> recentChange;
    bool** get_table();
    int get_size();
    void set(int x, int y, bool alive);
    void reset();
    void resize(int new_size);
private:
    int size;
    bool** values;
};


#endif //CONWAYSGAMEOFLIFE_CONWAY_HPP
