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
private:
    int size;
    bool** values;
};


#endif //CONWAYSGAMEOFLIFE_CONWAY_HPP
