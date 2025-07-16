//
// Created by seer on 2025/7/2.
//
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
using ll = long long;
using PDI = pair<ll, int>;

vector<ll> dijkstra(int s, vector<vector<pair<int, int> > > &adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    vector<ll> distance(n, 1e18);
    priority_queue<PDI, vector<PDI>, greater<> > q;
    q.emplace(0, s);
    distance[s] = 0;
    while (!q.empty()) {
        auto [d,u] = q.top();
        q.pop();
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        for (auto [v,w]: adj[u]) {
            if (distance[v] > d + w) {
                distance[v] = d + w;
                q.emplace(distance[v], v);
            }
        }
    }
    return distance;
}

// int main() {
//     int m, n, s, t;
//     cin >> n >> m >> s >> t;
//     vector<vector<pair<int, int> > > adj(n + 1);
//     for (int i = 0; i < m; ++i) {
//         int u, v;
//         ll w;
//         std::cin >> u >> v >> w;
//         adj[u].emplace_back(v, w);
//         adj[v].emplace_back(u, w);
//     }
//     const auto dist = dijkstra(s, adj);
//     std::cout << dist[t] << '\n';
//     return 0;
// }
