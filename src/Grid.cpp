//
// Created by seer on 2025/3/26.
//

#include "Grid.h"

Grid::Grid(int width, int height): width(width), height(height) {
    grid = std::vector(width, std::vector(height, 0));
    grid[1][1] = grid[1][2] = grid[1][3] = 1;
}

void Grid::setObstacle(int x, int y) {
    if (isValid(x, y)) {
        grid[x][y] = 1;
    }
}

void Grid::removeObstacle(int x, int y) {
    if (isValid(x, y)) {
        grid[x][y] = 0;
    }
}

bool Grid::isObstacle(int x, int y) const {
    if (isValid(x, y)) {
        return grid[x][y] == 1;
    }
}

bool Grid::isValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

int Grid::getHeight() const {
    return height;
}

int Grid::getWidth() const {
    return width;
}

void Grid::display() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << (grid[x][y] == 1 ? "#" : ".") << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int> > Grid::getGrid() const {
    return grid;
}
