//
// Created by seer on 2025/7/3.
//

#ifndef SHORTESTCIRCUIT_H
#define SHORTESTCIRCUIT_H
#include <common.h>
using namespace std;

class shortestCircuit {
public:
    struct Edge {
        int from, to;
        double weight;
    };

    using PDI = pair<double, int>;
    const double INF = std::numeric_limits<double>::infinity();

    struct Graph {
        vector<vector<Edge> > adj;

        void addEdge(int from, int to, double weight) {
            const Edge edge = {from, to, weight};
            adj[from].push_back(edge);
        }
    };


    struct Dijkstra {
        int V;
        const Graph &graph;
        vector<vector<double> > dist;
        vector<vector<int> > pred;

        explicit Dijkstra(const Graph &graph) : V(graph.adj.size()),
                                                graph(graph),
                                                dist(V, vector<double>(V, INF)),
                                                pred(V, vector<int>(V, -1)) {
            for (int i = 0; i < V; ++i) {
                dijkstra(i);
            }
        }


        void dijkstra(int s) {
            priority_queue<PDI, vector<PDI>, greater<PDI> > pq;
            vector<bool> vis(V, false);
            vector<double> &ds = dist[s];
            vector<int> &p = pred[s];
            ds[s] = 0;
            pq.push(make_pair(0.0, s));
            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();
                if (vis[u]) {
                    continue;
                }
                vis[u] = true;
                for (auto [f,t,w]: graph.adj[u]) {
                    if (ds[t] > w + ds[u]) {
                        ds[t] = w + ds[u];
                        p[t] = u;
                        pq.push(make_pair(ds[t], t));
                    }
                }
            }
        }

        pair<vector<int>, vector<double> > dijkstra_with_bellman(int s, vector<vector<Edge> > &edges) {
            priority_queue<PDI, vector<PDI>, greater<PDI> > pq;
            vector<bool> vis(V, false);
            vector<double> ds(V, INF);
            vector<int> p(V, -1);
            ds[s] = 0;
            pq.push(make_pair(0.0, s));
            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();
                if (vis[u]) {
                    continue;
                }
                vis[u] = true;
                for (auto [f,t,w]: edges[u]) {
                    if (ds[t] > w + ds[u]) {
                        ds[t] = w + ds[u];
                        p[t] = u;
                        pq.push(make_pair(ds[t], t));
                    }
                }
            }
            return make_pair(p, ds);
        }
    };

    struct Bellman {
        int V;
        const Graph &graph;
        vector<vector<double> > dist;
        vector<vector<int> > pred;

        explicit Bellman(const Graph &graph) : V(graph.adj.size()),
                                               graph(graph),
                                               dist(V, vector<double>(V, INF)),
                                               pred(V, vector<int>(V, -1)) {
        }

        //检查有无环
        bool bellman(vector<Edge> &edges) const {
            vector<double> d(V, INF);
            for (int i = 0; i < V - 1; ++i) {
                bool flag = false; //松弛操作
                for (int i = 0; i < V - 1; ++i) {
                    for (auto &[from,to,weight]: edges) {
                        if (d[from] < INF && d[to] > d[from] + weight) {
                            d[to] = d[from] + weight;
                            flag = true;
                        }
                    }
                    if (!flag) {
                        break;
                    }
                }
            }
            for (auto &[from,to,weight]: edges) {
                if (d[from] < INF && d[to] > d[from] + weight) {
                    return false;
                }
            }
            return true;
        }

        //Shortest Path Faster Algorithm
        bool spfa(int s) const {
            queue<int> q;
            vector<int> in_q_c(V, 0);
            vector<bool> in_q(V, false);
            vector<double> d(V, INF);
            d[s] = 0;
            q.push(s);
            in_q_c[s] = 1;
            in_q[s] = true;
            while (!q.empty()) {
                const int u = q.front();
                q.pop();
                in_q[u] = false;
                for (auto &[from,to,weight]: graph.adj[u]) {
                    if (d[u] < INF && d[to] > d[u] + weight) {
                        d[to] = d[u] + weight;
                        if (!in_q[to]) {
                            q.push(to);
                            ++in_q_c[to];
                            if (in_q_c[to] >= V) {
                                //出现负环
                                return false;
                            }
                        }
                    }
                }
            }
            return true;
        }
    };

    struct JohnsonAlgorithm {
        int V;
        const Graph &graph;
        vector<vector<double> > dist;
        vector<vector<int> > pred;

        explicit Johnson(const Graph &graph) : V(graph.adj.size()), graph(graph), dist(V, vector<double>(V, INF)),
                                               pred(V, vector<int>(V, -1)) {
        }

        void bellman_ford(vector<int> &h, vector<vector<Edge> > &adj) const {
            h.assign(V + 1, INF);
            h[V] = 0;
            for (int i = 0; i < V; ++i) {
                for (auto edge: graph.adj) {
                    for (auto &[from,to,weight]: edge) {
                        if (h[from] < INF && h[to] > h[from] + weight) {
                            h[to] = h[from] + weight;
                        }
                    }
                }
                for (auto edge: graph.adj) {
                    for (auto &[from,to,weight]: edge) {
                        const double v = h[to] - h[from] + weight;
                        adj[from].push_back({from, to, v});
                    }
                }
            }
        }

        void johnson(int s) {
            Bellman bellman(graph);
            if (!bellman.spfa(s)) {
                return;
            }
            vector<vector<Edge> > adj;
            vector<int> h;
            bellman_ford(h, adj);
            Dijkstra Dijkstra(graph);
            for (int i = 0; i < V; ++i) {
                auto &[p,ds] = Dijkstra.dijkstra_with_bellman(i, adj);
                vector<double> res = ds;
                for (int j = 0; j < ds.size(); ++j) {
                    res[j] = ds[j] + h[j] - h[i];
                }
                dist[i] = res;
                pred[i] = p;
            }
        }
    };
};


#endif //SHORTESTCIRCUIT_H
