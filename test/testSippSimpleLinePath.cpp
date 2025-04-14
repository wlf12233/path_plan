#include "sipp.h"
#include <iostream>
#include <cassert>

void testSippSimpleLinePath() {
    std::cout << "Running test: Simple Line Path..." << std::endl;

    int width = 6, height = 1;
    SIPP planner(width, height);

    for (int x = 0; x < width; ++x) {
        if (x != 3) {
            planner.addSafeInterval(x, 0, 0, 100);
        }
    }

    // 设置障碍：节点 (3,0) 在 [3,10] 是障碍，不可通行
    // 所以它只在 [0,3) 和 (10,100] 可以通行
    planner.addSafeInterval(3, 0, 0, 2.9); // 安全时间段 1
    planner.addSafeInterval(3, 0, 10.1, 100); // 安全时间段 2

    auto path = planner.findPath({0, 0}, {5, 0});

    if (path.empty()) {
        std::cout << " Failed to find path!\n";
    } else {
        std::cout << " Path found: \n";
        for (const auto &[x, y,t]: path) {
            std::cout << "(" << x << "," << y << ") at time " << t << "\n";
        }
        std::cout << "\n";
    }
}

int main(int argc, char *argv[]) {
    testSippSimpleLinePath();
    return 0;
}
