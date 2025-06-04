//
// Created by seer on 2025/6/3.
//

#include "AStarWithGraph.h"
#include "Johnson.h"

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
//     auto heuristic = [](int start, int goal) {
//         return 0.0; //退化为dijkstra
//     };
//     AStarWithGraph a_graph;
//     std::vector<double> dist;
//     std::vector<int> pred;
//     std::vector<std::vector<std::pair<int, double> > > adj(V);
//     for (auto [u,v,w]: graph.edges) {
//         adj[u].emplace_back(v, w);
//     }
//     a_graph.aStar(4, 0, 3, adj, dist, pred, heuristic);
//     std::vector<int> path;
//     for (int cur = 3; cur != -1; cur = pred[cur]) {
//         path.push_back(cur);
//     }
//     std::ranges::reverse(path);
//     for (int value : path) {
//         std::cout << value << " ";
//     }
// }
