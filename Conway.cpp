//
// Created by GPowers on 11/7/2022.
//

#include <iostream>
#include "Conway.hpp"


void Conway::tick() {
    bool** testBoard = new bool * [size];
    for(int x = 0; x < size; x++)
    {
        testBoard[x] = new bool[size];
        for (int y = 0;y < 100; ++y) {
            testBoard[x][y] = values[x][y];
        }
    }



    for(int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++) {
            int alive = 0;
            for(int x_offset = -1; x_offset < 2; x_offset++)
            {
                int cur_x = x + x_offset;
                if(x + x_offset < 0)
                {
                    cur_x = size-1;
                }
                if(x + x_offset >= size)
                {
                    cur_x = 0;
                }
                for (int y_offset = -1; y_offset < 2; y_offset++) {
                    int cur_y = y + y_offset;
                    if(x_offset == 0 && y_offset == 0)
                    {
                        continue;
                    }
                    if(y + y_offset < 0)
                    {
                        cur_y = size-1;
                    }
                    if(y + y_offset >= size)
                    {
                        cur_y = 0;
                    }
                    if(testBoard[cur_x][cur_y])
                    {
                        alive++;
                    }
                }
            }
            if(alive < 2 || alive > 3)
            {
                values[x][y] = false;
            }
            else if(alive == 3)
            {
                values[x][y] = true;
            }
        }
    }
    delete[] testBoard;
}

Conway::Conway(int max_size) {
    size = max_size;
    values = new bool * [max_size];
    for(int y = 0; y < max_size; y++)
    {
        values[y] = new bool[max_size];
        for (int x = 0; x < 100; ++x) {
            values[y][x] = false;
        }
    }
    values[50][50] = true;
    values[50][49] = true;
    values[50][48] = true;
    values[49][48] = true;
    values[48][49] = true;
}

bool** Conway::get_table() {
    return values;
}