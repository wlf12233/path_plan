//
// Created by seer on 2025/6/3.
//

#ifndef JOHNSON_H
#define JOHNSON_H
#include "common.h"
#include <limits>

const double INF = std::numeric_limits<double>::infinity();

struct Edge {
    int u, v;
    double weight;
};

class Johnson {
public:
    int V;
    std::vector<Edge> edges;
    std::vector<std::vector<double> > dist;
    std::vector<std::vector<int> > pred;

    explicit Johnson(int v): V(v) {
        dist.assign(V, std::vector<double>(V, INF));
        pred.assign(V, std::vector<int>(V, -1));
    }

    void addEdge(int u, int v, double weight) {
        edges.push_back({u, v, weight});
    }

    //执行 BellmanFord 算法，判断图中是否存在负权环
    bool bellmanFord(std::vector<double> &h) const {
        h.assign(V + 1, INF); //分配+1大小的距离数组
        h[V] = 0;

        std::vector<Edge> extendedEdges = edges;
        for (int i = 0; i < V; i++) {
            extendedEdges.push_back({V, i, 0});
        }
        // Bellman-Ford 主循环：对所有边松弛 V 次
        for (int i = 0; i < V; i++) {
            for (auto e: extendedEdges) {
                if (h[e.u] + e.weight < h[e.v]) {
                    h[e.v] = h[e.u] + e.weight; // 更新从 s 到 e.v 的最短距离
                }
            }
        }
        for (auto e: extendedEdges) {
            if (h[e.u] + e.weight < h[e.v]) {
                return false;
            }
        }
        return true;
    }

    bool SPFA(int src, std::vector<std::vector<Edge> > &adj, std::vector<double> &dist) {
        const int n = adj.size();
        dist.assign(n, INF);
        std::vector<int> cnt{n, 0};
        std::vector<bool> in_queue(n, false);
        std::queue<int> q;

        dist[src] = 0;
        q.push(src);
        in_queue[src] = true;
        cnt[src]++;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = true;
            for (auto [f,v,w]: adj[u]) {
                if (dist[u] + w < dist[f]) {
                    dist[f] = dist[u] + w;
                    if (!in_queue[f]) {
                        q.push(f);
                        in_queue[f] = true;
                        cnt[f]++;
                        if (cnt[f] > n) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    // 执行 Dijkstra 算法计算从源点 src 出发的最短路径（Johnson算法中的 Dijkstra）
    // adj 是重标定后的图（权值已被调整，全部为非负）
    // h 是通过 Bellman-Ford 得到的势函数，用于恢复原图中的最短路径距离
    void dijkstra(int src, std::vector<std::vector<std::pair<int, double> > > &adj, const std::vector<double> &h) {
        std::vector<double> d(V, INF);
        std::vector<int> p(V, -1);
        d[src] = 0;

        using PDI = std::pair<double, int>;
        std::priority_queue<PDI, std::vector<PDI>, std::greater<> > pq;
        pq.emplace(0.0, src);
        while (!pq.empty()) {
            auto [ds,u] = pq.top();
            pq.pop();
            if (ds > d[u]) {
                continue;
            }
            for (auto [v,w]: adj[u]) {
                if (d[v] > d[u] + w) {
                    d[v] = d[u] + w;
                    p[v] = u;
                    pq.emplace(d[v], v);
                }
            }
        }
        // 将 Dijkstra 得到的结果映射回原图的实际距离（通过 h[] 反向重标定）
        for (int v = 0; v < V; v++) {
            if (d[v] < INF) {
                dist[src][v] = d[v] - h[src] + h[v]; // Johnson 重标定恢复原图距离
            }
            pred[src][v] = p[v];
        }
    }

    bool run() {
        std::vector<double> h;
        if (!bellmanFord(h)) {
            return false;
        }
        std::vector<std::vector<std::pair<int, double> > > adj(V);
        for (const auto &[u, v, weight]: edges) {
            double w = weight + h[u] - h[v];
            adj[u].emplace_back(v, w);
        }
        for (int u = 0; u < V; u++) {
            dijkstra(u, adj, h);
        }
        return true;
    }

    void printDistances() const {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][j] == INF) {
                    std::cout << "INF" << std::endl;
                } else {
                    std::cout << i << " " << j << " " << dist[i][j] << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }

    std::vector<int> getPath(int u, int v) const {
        std::vector<int> path;
        if (dist[u][v] != INF) {
            for (int cur = v; cur != u; cur = pred[u][cur]) {
                path.push_back(cur);
            }
            path.push_back(u);
            std::ranges::reverse(path);
        }
        return path;
    }
};

#endif //JOHNSON_H
