//
// Created by seer on 2025/3/26.
//

#ifndef GRID_H
#define GRID_H
#include <vector>


class Grid {
public:
    Grid(int rows, int cols);

    void print();

    std::vector<std::vector<int> > getGrid() const;

private:
    int rows, cols;
    std::vector<std::vector<int> > grid;
};


#endif //GRID_H
