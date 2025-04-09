//
// Created by seer on 2025/3/26.
//

#include "Grid.h"
#include <iostream>

Grid::Grid(int rows, int cols): rows(rows), cols(cols) {
    grid = std::vector(rows, std::vector(cols, 0));
    grid[1][1] = grid[1][2] = grid[1][3] = 1;
}

void Grid::print() {
    for (auto &vector: grid) {
        for (int value: vector) {
            std::cout << (value ? "#" : ".") << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int> > Grid::getGrid() const {
    return grid;
}
