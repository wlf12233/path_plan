//
// Created by seer on 2025/7/14.
//

#ifndef MINCOSTMACFLOW_H
#define MINCOSTMACFLOW_H
#include <common.h>
using namespace std;
constexpr int INF = 1e9;

class MinCostFlow {
public:
    struct Edge {
        int to, rev, cap, cost; //cost 是单位流量费用
    };

    void add_edge(int u, int v, int cap, int cost) {
        adj[u].push_back({v, adj[v].size(), cap, cost});
        adj[v].push_back({u, adj[u].size() - 1, 0, -cost});
    }

    // 计算最小费用最大流
    // 参数：s 为源点，t 为汇点，maxf 为最大允许的流量
    // 返回值：{实际流量, 总费用}
    pair<int, int> flow(int s, int t, int maxf) {
        int flow = 0, cost = 0;
        while (flow < maxf) {
            //使用 SPFA 计算最短路径
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            vector<bool> inq(n, false);
            queue<int> q;
            q.push(s);
            inq[s] = true;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                inq[u] = false;
                for (int i = 0; i < adj[u].size(); ++i) {
                    for (auto [v,rev,cap,cost]: adj[u]) {
                        if (cap > 0 && dist[u] + cost < dist[v]) {
                            dist[v] = dist[u] + cost;
                            prevv[v] = u;
                            preve[v] = i;
                            if (!inq[v]) {
                                q.push(v);
                                inq[v] = true;
                            }
                        }
                    }
                }
            }
            if (dist[t] == INF) break;
            // 计算本次增广的流量（最小残量）
            int d = maxf - flow;
            for (int v = t; v != s; v = prevv[v]) {
                d = min(d, adj[prevv[v]][preve[v]].cap);
            }
            // 更新边和反向边的容量
            flow += d;
            cost += d * dist[t]; // 增加总费用
        }
        for (int v = t; v != s; v = prevv[v]) {
            auto edge = adj[prevv[v]][preve[v]];
            edge.cap -= flow;
            adj[v][edge.rev].cap += flow;
        }
        return {flow, cost};
    }

private:
    int n;
    vector<vector<Edge> > adj;
    vector<int> dist; //最短距离（费用）
    vector<int> prevv; //前驱节点
    vector<bool> preve; //前驱边索引
};


#endif //MINCOSTMACFLOW_H
