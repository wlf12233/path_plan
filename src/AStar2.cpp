//
// Created by seer on 2025/12/16.
//

#include "AStar2.h"

std::vector<int> AStar2::findPath(int source, int target, std::vector<Point> &graph) {
    const double INF = std::numeric_limits<double>::max();
    int n = graph.size();

    std::vector<bool> closed(n, false);
    std::vector<double> gScore(n, INF);
    std::vector<int> parent(n, -1);

    using PQNode = std::pair<double, int>; //f,id

    std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode> > pq;

    gScore[source] = 0.0;
    pq.push({dis[source][target], source});

    while (!pq.empty()) {
        const auto [f,u] = pq.top();
        pq.pop();

        if (closed[u]) {
            continue;
        }

        if (u == target) {
            std::vector<int> path;
            for (int v = u; v != -1; v = parent[v]) {
                path.push_back(v);
            }
            std::ranges::reverse(path);
            return path;
        }
        for (const auto &[v,w]: graph[u].edges) {
            closed[v] = true;
            double new_gScore = gScore[u] + w;
            if (new_gScore < gScore[v]) {
                gScore[v] = new_gScore;
                parent[v] = u;
                const double h = dis[v][target];
                pq.push({h + new_gScore, v});
            }
        }
    }
    return {};
}
