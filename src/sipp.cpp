//
// Created by seer on 2025/4/1.
//

#include "sipp.h"

SIPP::SIPP(int width, int height): width(width), height(height) {
    grid.resize(width, vector<vector<SafeInterval> >(height));
    moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
}

void SIPP::addSafeInterval(int x, int y, double start, double end) {
    grid[x][y].push_back({start, end});
}

std::vector<std::pair<int, int> > SIPP::findPath(std::pair<int, int> start, std::pair<int, int> goal) {

}

bool SIPP::isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

double SIPP::heuristic(pair<int, int> a, pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<pair<int, int> > SIPP::reconstructPath(State goal) {
    return {{goal.x, goal.y}};
}
int SIPP::hashState(int x, int y, int idx) {
    return x * 10000 + y * 100 + idx;
}

