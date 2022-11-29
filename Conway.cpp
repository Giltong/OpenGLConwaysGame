//
// Created by GPowers on 11/7/2022.
//

#include <iostream>
#include "Conway.hpp"
#include <unordered_set>

std::set<std::pair<int,int>> get_neighbors(std::set<std::pair<int,int>> current_alive)
{
    std::set<std::pair<int,int>> neighbors{};
    for(const auto& cell : current_alive)
    {
        for(int x_offset = -1; x_offset < 2; x_offset++)
        {
            for(int y_offset = -1; y_offset < 2; y_offset++)
            {
                if(x_offset == 0 && y_offset == 0)
                {
                    continue;
                }

                int x = cell.first + x_offset;
                int y =  cell.second + y_offset;

                if(current_alive.find({x,y}) == current_alive.end())
                {
                    neighbors.insert({x,y});
                }
            }
        }
    }

    return neighbors;
}

void Conway::tick() {

    std::set<std::pair<int,int>> cur_alive_cells = alive_cells;
    neighbors = get_neighbors(cur_alive_cells);
    std::set<std::pair<int,int>> cells_to_check;
    cells_to_check.insert(cur_alive_cells.begin(), cur_alive_cells.end());
    cells_to_check.insert(neighbors.begin(), neighbors.end());
        for(const auto& point : cells_to_check)
        {
            int alive = 0;
            int x = point.first;
            int y = point.second;

            for(int x_offset = -1; x_offset < 2; x_offset++)
            {
                int cur_x = x + x_offset;
                for (int y_offset = -1; y_offset < 2; y_offset++) {
                    int cur_y = y + y_offset;
                    if(x_offset == 0 && y_offset == 0)
                    {
                        continue;
                    }
                    auto k = cur_alive_cells.find({cur_x, cur_y});
                    if(k != cur_alive_cells.end())
                    {
                        alive++;
                    }
                }
            }
            if(alive < minPop || alive > maxPop)
            {
                alive_cells.erase({x,y});
            }
            else if(alive == reproductionPop && cur_alive_cells.find({x, y}) == cur_alive_cells.end())
            {
                alive_cells.insert({x,y});
            }
        }
}

std::set<std::pair<int,int>>& Conway::get_table() {
    return alive_cells;
}


void Conway::set(int x, int y, bool alive) {

}

void Conway::reset() {
    alive_cells.clear();
    neighbors.clear();
}
