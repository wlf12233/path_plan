//
// Created by seer on 2025/4/14.
//
#include "AStar.h"
#include "Grid.h"
#include <iostream>
#include <cassert>

void testAstarSimpleLinePath() {
    // <b>lang</b> variable name to see how CLion can help you rename it.
    Grid grid(10, 10);
    AStar astar(10, 10, manhattan);

    grid.display();

    auto start_time = std::chrono::high_resolution_clock::now();

    if (auto path = astar.find_path_1(grid.getGrid(), {0, 0}, {4, 4})) {
        std::cout << "路径找到: ";
        for (const auto &[x, y]: path.value()) {
            std::cout << "(" << x << "," << y << ") -> ";
        }
        std::cout << "终点\n";
    } else {
        std::cout << "未找到路径\n";
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end_time - start_time;
    std::cout << diff.count() << " seconds\n";
}

