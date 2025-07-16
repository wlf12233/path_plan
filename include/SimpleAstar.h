//
// Created by seer on 2025/7/4.
//

#ifndef SIMPLEASTAR_H
#define SIMPLEASTAR_H
#include <common.h>

#include "Johnson.h"
using namespace std;

class SimpleAstar {
public:
    struct Edge {
        int u, v;
        int w;
    };

    int V, E;
    vector<Edge> edges;
    vector<vector<Edge> > adj;

    //构建反图，计算最佳的 h
    void dijkstra(int t, vector<int> &h, const vector<vector<Edge> > &reverse_edges) {
        ranges::fill(h, INF);
        h[t] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<> > q;
        q.push(make_pair(0, t));
        while (!q.empty()) {
            auto [d,u] = q.top();
            q.pop();
            if (d > h[u]) { continue; }
            for (auto &[u,v,w]: reverse_edges[u]) {
                if (h[v] > h[u] + w) {
                    h[v] = h[u] + w;
                    q.push(make_pair(h[v], v));
                }
            }
        }
    }

    vector<int> getPath(int start, int end, const function<int(int, int)> &h) const {
        const int INF = numeric_limits<int>::max();
        int n = adj.size();
        vector<int> g(n, INF);
        vector<int> p(n, -1);
        using PDI = pair<double, int>;
        priority_queue<PDI, vector<PDI>, greater<PDI> > q;
        q.push(make_pair(h(start, end), start));
        g[start] = 0;
        while (!q.empty()) {
            auto [f,u] = q.top();
            q.pop();
            if (u == end) {
                break;
            }
            for (auto edge: adj[u]) {
                int v = edge.v;
                int g2 = edge.w + g[u];
                if (g2 < g[v]) {
                    g[v] = g2;
                    p[v] = u;
                    q.push(make_pair(h(v, end) + g2, v));
                }
            }
        }
        if (g[end] == INF) {
            return {};
        }
        vector<int> path;
        while (p[end] != start) {
            path.push_back(p[end]);
            end = p[p[end]];
        }
        ranges::reverse(path);
        return path;
    }

    int astar_kth(int s, int t, int k, vector<int> &h) const {
        if (s == t) {
            k++;
        }
        vector<int> g(adj[s].size(), INF);
        vector<int> p(adj[t].size(), -1);
        using PDI = pair<double, int>;
        priority_queue<PDI, vector<PDI>, greater<PDI> > q;
        q.push(make_pair(h[s], s));
        int cur = 0;
        while (!q.empty()) {
            auto [f,u] = q.top();
            q.pop();
            if (u == t) {
                cur++;
                if (cur == k) {
                    return f; //或者g
                }
            }
            for (auto edge: adj[u]) {
                q.push(make_pair(h[edge.v] + g[edge.v] + edge.w, edge.v));
            }
        }
        return -1;
    }
};


#endif //SIMPLEASTAR_H
