//
// Created by seer on 2025/4/27.
//

#include "CBS.h"
#include <iostream>
#include <vector>

void testCBSSimpleLinePath() {
    std::vector<std::vector<int>> map = {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    // 示例起点和终点
    std::vector<std::pair<int, int>> starts = {{0, 0}, {0,1}}; // 代理1和代理2的起点
    std::vector<std::pair<int, int>> goals = {{0,2}, {0, 0}};  // 代理1和代理2的目标
    CBS cbs(starts,goals,map);

    // 检查结果
    if (auto result=cbs.solve()) {
        std::cout << "Paths found:" << std::endl;
        for (size_t i = 0; i < result->size(); ++i) {
            std::cout << "Agent " << i + 1 << " path:" << std::endl;
            for (const auto&[fst, snd] : result->at(i)) {
                std::cout << "(" << fst << ", " << snd << ") ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No solution found." << std::endl;
    }

}
// int main() {
//     testCBSSimpleLinePath();
//     return 0;
// }