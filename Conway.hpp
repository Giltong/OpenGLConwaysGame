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
    bool** get_table();
    int get_size();
    void set(int x, int y, bool alive);
    void reset();
    void resize(int new_size);
public:
    int maxPop = 3;
    int minPop = 2;
    int reproductionPop = 3;
private:
    int size;
    bool** values;

};


#endif //CONWAYSGAMEOFLIFE_CONWAY_HPP
