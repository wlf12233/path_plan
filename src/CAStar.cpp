//
// Created by seer on 2025/12/16.
//

#include "CAStar.h"

std::vector<int> CAStar::findPath(int source, int target, std::vector<Point> &graph) {
    const int n = graph.size();
    constexpr int TMax = 10;
    constexpr double INF = std::numeric_limits<double>::infinity();
    std::vector<std::vector<bool> > closed(n, std::vector<bool>(TMax, false));
    std::vector<std::vector<double> > gScore(n, std::vector<double>(TMax, INF));
    // pred[v][t] = {prev_v, prev_t}
    std::vector<std::vector<std::pair<int, int> > > pred(n, std::vector<std::pair<int, int> >(TMax, {-1, -1}));
    std::priority_queue<STATE, std::vector<STATE>, Cmp> pq;

    gScore[source][0] = 0.0;
    pq.push({source, 0, 0.0, dis[source][target]});
    while (!pq.empty()) {
        const STATE cur = pq.top();
        pq.pop();
        int t = cur.t;
        int v = cur.v;
        if (closed[v][t]) continue;
        if (t > TMax) continue;

        closed[v][t] = true;
        if (v == target) {
            std::vector<int> paths;
            while (v != -1) {
                paths.push_back(v);
                auto [pv,pt] = pred[target][t];
                v = pv;
                t = pt;
            }
            std::ranges::reverse(paths.begin(), paths.end());
            return paths;
        }
        // 移动到邻居
        for (const auto &[to,w]: graph[v].edges) {
            const int nt = t + w;
            if (nt > TMax) {
                continue;
            }
            double ng = gScore[v][t] + w;
            if (ng < gScore[to][nt]) {
                gScore[to][nt] = ng;
                pred[to][nt] = {v, t};
                double h = dis[to][target];
                pq.push({to, nt, ng, h + ng});
            }
        }
        // 等待
        int nt = t + 1;
        if (nt > TMax) {
            continue;
        }
        double ng = gScore[v][t] + 1.0;
        if (ng < gScore[v][nt]) {
            gScore[v][nt] = ng;
            pred[v][nt] = {v, t};
            double h = dis[v][target];
            pq.push({v, nt, ng, h + ng});
        }
    }
    return {};
}

std::vector<CAStar::TimeNode> CAStar::findPathWithRT(int source, int target, std::vector<Point> &graph, ReservationTable &rt) {
    int n = graph.size();
    int TMax = 10;
    double INF = std::numeric_limits<double>::infinity();
    std::vector<std::vector<bool> > closed(n, std::vector<bool>(TMax, false));
    std::vector<std::vector<double> > gScore(n, std::vector<double>(TMax, INF));
    std::vector<std::vector<std::pair<int, int> > > pred(n, std::vector<std::pair<int, int> >(TMax, {-1, -1}));
    gScore[source][0] = 0.0;
    std::priority_queue<STATE, std::vector<STATE>, Cmp> pq;
    pq.push({source, 0, 0.0, dis[source][target]});
    while (!pq.empty()) {
        const STATE cur = pq.top();
        pq.pop();
        int t = cur.t;
        int u = cur.v;
        if (u == target) {
            std::vector<TimeNode> paths;
            while (u != -1) {
                paths.push_back({u, t});
                auto [pv,pt] = pred[u][t];
                u = pv;
                t = pt;
            }
            std::ranges::reverse(paths.begin(), paths.end());
            return paths;
        }
        if (closed[u][t]) { continue; }
        if (t > TMax) { continue; }
        closed[u][t] = true;
        for (const auto &[v,w]: graph[u].edges) {
            const int nt = t + w;
            if (nt > TMax) {
                continue;
            }
            if (rt.isEdgeReserved(u, v, t)) continue;
            if (rt.isReserved(v, nt)) continue;
            double ng = gScore[u][t] + w;
            if (ng < gScore[v][nt]) {
                gScore[v][nt] = ng;
                pred[v][nt] = {u, t};
                double h = dis[v][target];
                pq.push({u, nt, ng, h + ng});
            }
        }
        int nt = t + 1;
        if (nt > TMax) {
            continue;
        }
        double ng = gScore[u][t] + 1.0;
        if (ng < gScore[u][nt]) {
            gScore[u][nt] = ng;
            pred[u][nt] = {u, t};
            double h = dis[u][target];
            pq.push({u, nt, ng, h + ng});
        }
    }
    return {};
}

void CAStar::reserverPath(std::vector<TimeNode> &paths, ReservationTable &rt) {
    for (int i = 0; i < paths.size(); i++) {
        rt.vertex.insert(rt.vkey(paths[i].v, paths[i].t));
        if (i > 0) {
            rt.edge.insert(rt.ekey(paths[i].v, paths[i - 1].v, paths[i].t));
        }
    }
}
