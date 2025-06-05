//
// Created by seer on 2025/6/3.
//
#include "Johnson.h"
//
// int main() {
//     int V = 4;
//     Johnson graph(V);
//
//     // 添加图边
//     graph.addEdge(0, 1, 4);
//     graph.addEdge(0, 2, 1);
//     graph.addEdge(2, 1, -2);
//     graph.addEdge(1, 3, 2);
//     graph.addEdge(2, 3, 5);
//
//     if (!graph.run()) {
//         std::cout << "图中存在负环！Johnson算法失败。\n";
//         return 0;
//     }
//
//     // 测试路径：从 0 到 3
//     int src = 0, target = 3;
//
//     double d = graph.dist[src][target];
//     if (d == INF) {
//         std::cout << "节点 " << src << " 到 " << target << " 不可达。\n";
//     } else {
//         std::cout << "最短路径距离：" << d << "\n";
//         auto path = graph.getPath(src, target);
//
//         std::cout << "路径为：";
//         for (int i = 0; i < path.size(); ++i) {
//             std::cout << path[i];
//             if (i + 1 < path.size()) std::cout << " → ";
//         }
//         std::cout << "\n";
//     }
// }
