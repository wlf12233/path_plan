//
// Created by seer on 2025/6/30.
//

#ifndef BIPARTITEMATCH_H
#define BIPARTITEMATCH_H
#include <common.h>
using namespace std;

class BipartiteMatch {
    struct Kuhn {
        /**
         * @param u 当前左侧点
         * @param adj 邻接表
         * @param match match[v] == 左侧编号，-1表示右侧点V未匹配
         * @param vis 本轮DFS中右侧点V是否已访问
         * @return 是否找到增光路
         */
        bool dfs(int u, const vector<vector<int> > &adj, vector<int> &match,
                 vector<bool> &vis) {
            for (const int edge: adj[u]) {
                if (vis[edge]) {
                    continue;
                }
                vis[edge] = true;
                // 若 v 未匹配，或 v 当前匹配的左侧点还能往下重新安排
                if (match[edge] == -1 || dfs(match[edge], adj, match, vis)) {
                    match[edge] = u;
                    return true;
                }
            }
            return false;
        }

        /**
         *  Kuhn 算法求二分图最大匹配
         * @param left  左侧点数（假设编号 1..n_left）
         * @param right 右侧点数（假设编号 1..n_right）
         * @param adj
         * @return 最大匹配数
         */
        int kuhn(int left, int right, const vector<vector<int> > &adj) {
            vector<int> match(right + 1, -1);
            vector<bool> vis(right + 1, false);
            int res = 0;
            for (int i = 1; i <= left; ++i) {
                fill(vis.begin(), vis.end(), false); //清空访问标记
                if (dfs(i, adj, match, vis)) {
                    ++res; //找到增广
                }
            }
            return res;
        }
    };

    struct HopcroftKarp {
        int left, right;
        vector<vector<int> > adj;
        vector<int> pair_u; // pair_u[u] = v，表示左侧点 u 匹配右侧点 v，0 表示未匹配
        vector<int> pair_v; // pair_v[v] = u，表示右侧点 v 匹配左侧点 u，0 表示未匹配
        vector<int> dist; // BFS 分层图中左侧点的层次距离（用于限制 DFS 范围）

        //BFS构造分层图
        bool bfs() {
            queue<int> q;
            for (int i = 1; i <= left; ++i) {
                if (pair_u[i] == 0) {
                    q.push(i); // 未匹配的左侧点，作为起点
                    dist[i] = 0;
                } else {
                    dist[i] = INT_MAX; // 已匹配的点初始化为无穷远
                }
            }
            bool found = false; // 是否找到任何右侧未匹配点
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v: adj[u]) {
                    int matched_u = pair_v[v]; //v当前匹配的左侧点
                    if (matched_u == 0) {
                        found = true;
                        // 还没匹配右侧点的左侧点
                    } else if (dist[matched_u] == INT_MAX) {
                        // 将 matched_u 加入下一层
                        dist[matched_u] = dist[u] + 1;
                        q.push(matched_u);
                    }
                }
            }
            return found; // 若找到了右侧未匹配点，则存在增广路径
        }

        // DFS：在分层图中查找增广路径
        bool dfs(int u) {
            for (int v: adj[u]) {
                int matched_u = pair_v[v];
                // 如果 v 没有匹配，或者它匹配的点 matched_u 可以再往下找到路径
                if (matched_u == 0 || (dist[matched_u] == dist[u] + 1 && dfs(matched_u))) {
                    pair_u[u] = v;
                    pair_v[v] = u;
                    return true;
                }
            }
            dist[u] = INT_MAX; // 没找到路径，标记为不可达
            return false;
        }

        int max_matching() {
            int matching = 0;
            while (bfs()) {
                for (int i = 1; i <= left; ++i) {
                    if (pair_u[i] == 0 && dfs(i)) {
                        ++matching;
                    }
                }
            }
            return matching;
        }
    };

    /*****************************************************
     *  Kuhn–Munkres / Hungarian Algorithm
     *  最大权完美匹配（带权二分图）
     *  复杂度：O(n^3)
    *****************************************************/
    struct Kuhn_Munkres {
    public:
        int n; //左侧/右侧的点数（必须相等）
        vector<vector<int> > w;
        vector<int> lx, ly; //顶标 Label
        vector<int> match; // 右侧的点匹配的左侧点
        vector<int> slack; // slack[i]=离相等边还差多少
        vector<bool> visX, visY;

        bool dfs(int x) {
            visX[x] = true;
            for (int i = 1; i <= n; ++i) {
                if (visY[i]) {
                    continue;
                }
                int delta = lx[x] + ly[x] - w[x][i]; // 距离相等边还差多少
                if (delta == 0) {
                    visY[i] = true;
                    if (match[i] == 0 || dfs(match[i])) {
                        match[i] = x;
                        return true;
                    }
                } else {
                    slack[i] = min(slack[i], delta);
                }
            }
            return false;
        }

        long long KM() {
            for (int i = 1; i <= n; ++i) {
                lx[i] = w[i][1];
                for (int j = 2; j <= n; ++j) {
                    lx[i] = max(lx[i], w[i][j]);
                }
            }
            memset(ly, 0, sizeof(ly));
            memset(match, 0, sizeof(match));

            for (int i = 1; i <= n; ++i) {
                ranges::fill(slack, INT_MAX);
                while (true) {
                    memset(visX, 0, sizeof(visX));
                    memset(visY, 0, sizeof(visY));
                    if (dfs(i)) { break; }
                    //未成功，松弛
                    int d = INT_MAX;
                    for (int i = 1; i <= n; ++i) {
                        if (!visY[i]) {
                            d = min(d, slack[i]);
                        }
                    }
                    for (int i = 1; i <= n; ++i) {
                        if (visX[i]) lx[i] -= d;
                        if (visY[i]) ly[i] += d;
                        else slack[i] -= d;
                    }
                }
            }
            long long res = 0;
            for (int i = 1; i <= n; ++i) {
                if (match[i]) {
                    res += w[match[i]][i];
                }
            }
            return res;
        }
    };
};
#endif //BIPARTITEMATCH_H
