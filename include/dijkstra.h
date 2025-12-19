//
// Created by seer on 2025/12/16.
//

#ifndef PATH_PLAN_DIJK_H
#define PATH_PLAN_DIJK_H
#include <common.h>

#include "Graph.h"


class dijkstra {
public:
    std::vector<std::vector<double> > dis;
    std::vector<std::vector<int> > pred;

    dijkstra() = default;

    void dijkstraPath(const std::vector<Point> &graph, int source, std::vector<double> &dis, std::vector<int> &pred) {
        const int n = graph.size();
        using PDI = std::pair<double, int>;
        constexpr double INF = std::numeric_limits<double>::infinity();

        dis.assign(n, INF);
        pred.assign(n, -1);

        dis[source] = 0.0;

        std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > pq;
        pq.push(std::make_pair(0.0, source));

        while (!pq.empty()) {
            auto [d,u] = pq.top();
            pq.pop();
            if (d > dis[u]) {
                continue;
            }
            for (auto [to,w]: graph[u].edges) {
                if (dis[to] < INF && dis[to] > dis[u] + w) {
                    dis[to] = dis[u] + w;
                    pred[to] = u;
                    pq.push(std::make_pair(dis[to], to));
                }
            }
        }
    }

    void computeAllPaths(const std::vector<Point> &graph) {
        int n = graph.size();
        dis.resize(n);
        pred.resize(n);

        for (int i = 0; i < n; ++i) {
            dijkstraPath(graph, i, dis[i], pred[i]);
        }
    }

    std::vector<int> reconstructPath(int source, int dest, std::vector<int> &prev) {
        std::vector<int> path;
        while (dest != -1) {
            path.push_back(dest);
            dest = prev[dest];
        }
        std::ranges::reverse(path.begin(), path.end());
        if (path.front() == source) {
            return path;
        }
        return {};
    }
};


#endif //PATH_PLAN_DIJK_H
