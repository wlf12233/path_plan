//
// Created by seer on 2025/5/28.
//

#ifndef ADJMATRIXGRAPG_H
#define ADJMATRIXGRAPG_H


#include "common.h"


class AdjMatrixGraph {
public:
    static constexpr double INF = std::numeric_limits<double>::infinity();

    explicit AdjMatrixGraph(int vertices): V(vertices), adj(vertices, std::vector<double>(vertices, INF)) {
        for (int i = 0; i < V; i++) {
            adj[i][i] = 0;
        }
    }

    void addEdge(int u, int v, double w) {
        adj[u][v] = w;
    }

    const std::vector<std::vector<double> > &getAdjMatrix() {
        return adj;
    }

    int getVertexCount() const {
        return V;
    }

    //返回从 src 到其他节点的最短距离
    std::vector<double> dijkstra(int source) const {
        std::vector<double> dist(V, INF); //最短距离初始化为无穷大
        std::vector<bool> visited(V, false); //标记是否已经访问
        dist[source] = 0.0;

        using PDI = std::pair<double, int>;
        std::priority_queue<PDI, std::vector<PDI>, std::greater<> > pq;
        pq.emplace(0.0, source);
        while (!pq.empty()) {
            auto [d,u] = pq.top();
            pq.pop();
            if (visited[u]) {
                continue;
            }
            visited[u] = true;
            for (auto v: adj[u]) {
                int weight = adj[u][v];
                if (weight < INF && weight + dist[u] < dist[v]) {
                    dist[v] = weight + dist[u];
                    pq.emplace(weight, v);
                }
            }
        }
        return dist;
    }

    //返回从 src 到其他节点的最短距离和具体路径
    std::pair<std::vector<double>, std::vector<int> > dijkstra2(int source) const {
        std::vector dist(V, INF); //最短距离初始化为无穷大
        std::vector visited(V, false); //标记是否已经访问
        std::vector pred(V, -1); //记录的路径
        dist[source] = 0.0;

        using PDI = std::pair<double, int>;
        std::priority_queue<PDI, std::vector<PDI>, std::greater<> > pq;
        pq.emplace(0.0, source);
        while (!pq.empty()) {
            auto [d,u] = pq.top();
            pq.pop();
            if (visited[u]) {
                continue;
            }
            visited[u] = true;
            for (int v = 0; v < V; v++) {
                double weight = adj[u][v];
                if (weight < INF && weight + dist[u] < dist[v]) {
                    dist[v] = weight + dist[u];
                    pred[v] = u; //记录前驱节点
                    pq.emplace(dist[v], v);
                }
            }
        }
        return std::make_pair(dist, pred);
    }

    void dijkstraConstrct(int source, std::vector<std::vector<double> > &dist, std::vector<std::vector<int> > &pred) {
        dist[source][source] = 0.0;
        using PDI = std::pair<double, int>;
        std::priority_queue<PDI, std::vector<PDI>, std::greater<> > pq;
        pq.emplace(0.0, source);
        while (!pq.empty()) {
            auto [d,u] = pq.top();
            pq.pop();
            for (const double edge: adj[u]) {
                if (dist[source][edge] > dist[source][u] + adj[source][u]) {
                    dist[source][edge] = dist[source][u] + adj[source][u];
                    pred[source][edge] = u;
                    pq.emplace(dist[source][edge], edge);
                }
            }
        }
    }

    std::vector<int> reconstructPath(int source, int dest, std::vector<int> &prev) const {
        std::vector<int> path;
        while (dest != -1) {
            path.push_back(dest);
            if (prev[dest] == source) {
                break;
            }
            dest = prev[dest];
            // std::cout<<dest<<" ";
        }
        // for (int at = dest; at != -1; at = prev[at]) {
        //     path.push_back(at);
        // }
        std::ranges::reverse(path);
        if (path.front() == source) {
            return path;
        }
        return {};
    }

private:
    int V;
    std::vector<std::vector<double> > adj; //邻接矩阵
};


#endif //ADJMATRIXGRAPG_H
