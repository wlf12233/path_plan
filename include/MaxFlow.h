//
// Created by seer on 2025/7/10.
//

#ifndef MAXFLOW_H
#define MAXFLOW_H
#include <common.h>

#include "Johnson.h"

using namespace std;

class maxflow {
public:
    struct Edge {
        int to, cap, rev;
    };

    int V;
    vector<vector<Edge> > adj;


    void addEdge(int u, int v, int cap) {
        adj[u].push_back({v, cap, adj[v].size()});
        adj[v].push_back({u, 0, adj[u].size() - 1});
    }

    struct FF {
        int V;
        vector<bool> visited;

        int dfs(int s, int t, int f) {
            if (s == t) return f;
            visited[s] = true;
            for (auto &edge: adj[s]) {
                if (!visited[edge.to] && edge.cap > 0) {
                    int d = dfs(edge.to, edge.cap, min(edge.cap, f));
                    if (d > 0) {
                        edge.cap -= d;
                        adj[edge.to][edge.rev].cap += d;
                        return d;
                    }
                }
            }
        }

        int max_flow(int S, int T) {
            int flow = 0;
            while (true) {
                visited.assign(V, false);
                const int d = dfs(S, T, INF);
                if (d == 0) break;
                flow += d;
            }
            return flow;
        }
    };

    struct EK {
        vector<int> pre;
        int V;
        vector<bool> vis;

        bool bfs(int s, int t, vector<int> &preV, vector<int> &preE) {
            queue<int> q;
            q.push(s);
            preV.assign(V, -1);
            preE.assign(V, -1);
            vis.assign(V, false);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int i = 0; i < adj[u].size(); ++i) {
                    auto edge = adj[u][i];
                    int v = edge.to;
                    if (!vis[v] && edge.cap > 0) {
                        vis[v] = true;
                        preV[v] = u;
                        preE[v] = i;
                        if (v == t) {
                            return true;
                        }
                        q.push(v);
                    }
                }
            }
            return false;
        }

        int max_flow(int S, int T) {
            int flow = 0;
            vector<int> preV(V);
            vector<int> preE(V);
            while (bfs(S, (T), preV, preE)) {
                int cur = T;
                int f = INF;
                while (cur != S) {
                    int u = preV[cur];
                    int edge = preE[cur];
                    f = min(f, adj[u][edge].cap);
                    cur = u;
                }
                cur = T;
                while (cur != S) {
                    auto edge = adj[preV[cur]][preE[cur]];
                    edge.cap -= f;
                    adj[cur][edge.rev].cap += f;
                }
                flow += f;
            }
            return flow;
        }
    };

    struct Dinic {
        vector<int> levels;
        int V;
        vector<int> iter; // 当前节点遍历到哪条边，用于加速 DFS

        bool bfs(int s, int t) {
            levels.assign(V, -1);
            queue<int> q;
            levels[s] = 0;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (auto &[to,cap,rev]: adj[u]) {
                    if (levels[to] < 0 && cap > 0) {
                        levels[to] = levels[u] + 1;
                        q.push(to);
                    }
                }
            }
            return levels[t] != -1;
        }

        int dfs(int s, int t, int f) {
            if (s == t) return f;
            for (int &i = iter[s]; i < adj[s].size(); ++i) {
                auto &e = adj[s][i];
                if (e.cap > 0 && levels[e.to] == levels[s] + 1) {
                    int d = dfs(e.to, e.to, min(e.cap, f));
                    if (d > 0) {
                        e.cap -= d;
                        adj[e.to][e.rev].cap += d;
                        return d;
                    }
                }
            }
            return 0;
        }

        int dinic(int s, int t) {
            int flow = 0;
            while (bfs(s, t)) {
                ranges::fill(iter, 0);
                int f;
                while ((f = dfs(s, t, INF))) {
                    flow += f;
                }
            }
            return flow;
        }
    };
};

#endif //MAXFLOW_H
