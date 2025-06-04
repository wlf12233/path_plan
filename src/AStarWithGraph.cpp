//
// Created by seer on 2025/6/3.
//

#include "AStarWithGraph.h"

bool AStarWithGraph::aStar(int V, int start, int goal, std::vector<std::vector<std::pair<int, double> > > adj, std::vector<double> &out_list,
                           std::vector<int> &out_pred, Heuristic heuristic) {
    out_list.assign(V,HUGE_VAL);
    out_pred.assign(V, -1);
    out_list[start] = 0;

    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<> > pq;
    pq.emplace(heuristic(start, goal), start);
    while (!pq.empty()) {
        auto [f, u] = pq.top();
        pq.pop();
        if (u == goal) {
            return true;
        }
        for (auto [v,w]: adj[u]) {
            if (out_list[v] > out_list[u] + w) {
                out_list[v] = out_list[u] + w;
                out_pred[v] = u;
                pq.emplace(heuristic(v, goal), v);
            }
        }
    }
    return false;
}
