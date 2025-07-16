//
// Created by seer on 2025/6/24.
//

#ifndef NETWORKFLOW_H
#define NETWORKFLOW_H
#include <common.h>

class NetworkFlow {
    struct Dinic {
        int N;
        std::vector<int> level; //各层的节点
        std::vector<int> iter; // 当前dfs访问的边的位置

        explicit Dinic(int n): N(n) {
        }

        void bfs(int s) {
            level.assign(N, -1);
            std::queue<int> q;
            level[s] = 0;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (auto v: edges[u]) {
                    if (v.cap > 0 && level[v.to] < 0) {
                        level[v.to] = level[u] + 1;
                        q.push(v.to);
                    }
                }
            }
        }

        int dfs(int v, int t, int upTo) {
            if (v == t) return upTo;
            for (int &i = iter[v]; i < edges[v].size(); ++i) {
                auto &e = edges[v][i];
                if (e.cap > 0 && level[v] < level[e.to]) {
                    int d = dfs(e.to, t, std::min(upTo, e.cap));
                    if (d > 0) {
                        e.cap -= d;
                        edges[e.to][e.rev].cap += d;
                        return d;
                    }
                }
            }
        }

        int max_flow(int s, int t) {
            int flow = 0;
            while (true) {
                bfs(s);
                if (level[t] < 0) break;
                iter.assign(N, 0);
                int f;
                while ((f = dfs(s, t, INT_MAX)) > 0) {
                    flow += f;
                }
            }
            return flow;
        }
    };

public:
    struct Edge {
        int to;
        int cap;
        int rev; //对应的反向边在邻接表中的位置
        int flow; //当前已经推送的流量
    };

    std::vector<std::vector<Edge> > edges;
    std::vector<bool> visited;
    int V;

    void addEdge(int u, int v, int cap) {
        edges[u].push_back({v, cap, edges[v].size()});
        edges[v].push_back({u, 0, edges[u].size() - 1});
    }

    //FF
    int dfs(int v, int t, int f) {
        if (v == t) return f;
        visited[v] = true;
        for (auto &[to, cap, rev,flow]: edges[v]) {
            if (!visited[to] && cap > 0) {
                int d = dfs(to, t, std::min(f, cap));
                if (d > 0) {
                    cap -= d;
                    edges[to][rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    //Edmonds-Karp
    int bfs(int s, int t, std::vector<int> &parent, std::vector<int> &parent_edge) {
        std::queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < edges[u].size(); ++i) {
                auto &e = edges[u][i];
                if (!visited[e.to] && e.cap - e.flow > 0) {
                    parent[e.to] = u;
                    parent_edge[e.to] = i;
                    visited[e.to] = true;
                    if (e.to == t) return true;
                    q.push(e.to);
                }
            }
        }
        return false;
    }

    //bfs
    int max_flow(int s, int t) {
        int flow = 0;
        std::vector<int> parent(V), parent_edge(V);

        while (bfs(s, t, parent, parent_edge)) {
            // 找到路径后，推送瓶颈流
            int cur = t;
            int f = INT_MAX;

            while (cur != s) {
                int prev = parent[cur];
                int ei = parent_edge[cur];
                f = std::min(f, edges[prev][ei].cap - edges[prev][ei].flow);
                cur = prev;
            }
            //更新正向/反向边
            while (cur != s) {
                int prev = parent[cur];
                int ei = parent_edge[cur];
                edges[prev][ei].flow += f;
                int rev = edges[prev][ei].rev;
                edges[cur][rev].flow -= f;
                cur = prev;
            }
            flow += f;
        }
        return flow;
    }

    //dfs
    int maxFlow(int s, int t) {
        int flow = 0;
        while (true) {
            visited.assign(V, false);
            int f = dfs(s, t,INT_MAX);
            if (f == 0) break;
            flow += f;
        }
        return flow;
    }
};

#endif //NETWORKFLOW_H
