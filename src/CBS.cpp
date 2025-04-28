//
// Created by seer on 2025/4/14.
//

#include "CBS.h"
#include "AStar.h"

CBS::CBS(const std::vector<std::pair<int, int> > &starts, const std::vector<std::pair<int, int> > &goals,
         const std::vector<std::vector<int> > &map)
    : starts(starts),
      goals(goals),
      map(map) {
}

std::optional<std::vector<Path> > CBS::solve() {
    std::priority_queue<CBSNode, std::vector<CBSNode>, std::greater<> > pq;
    CBSNode root;
    root.cost = 0;
    for (int i = 0; i < starts.size(); i++) {
        Path path = aStarWithConstrains(i, starts[i], goals[i], root.constraints);
        if (path.empty()) {
            return std::nullopt;
        }
        root.paths.push_back(path);
        root.cost += path.size();
    }
    pq.push(root);
    while (!pq.empty()) {
        const CBSNode node = pq.top();
        pq.pop();

        auto conflict = findConflict(node.paths);

        if (!conflict.has_value()) {
            return node.paths;
        }
        auto [a1,a2,x,y,t] = conflict.value();
        for (const int agent: {a1, a2}) {
            CBSNode child = node;
            child.constraints.push_back({agent, x, y, t});
            Path new_path = aStarWithConstrains(agent, starts[agent], goals[agent], child.constraints);
            if (new_path.empty()) {
                continue;
            }
            child.paths[agent] = new_path;
            child.cost = computeTotalCost(child.paths);
            pq.push(child);
        }
    }
    return std::nullopt;
}

std::optional<Conflict> CBS::findConflict(const std::vector<Path> &paths) {
    int max_len = 0;
    for (const auto &p: paths) {
        max_len = std::max(max_len, static_cast<int>(p.size()));
    }
    for (int t = 0; t < max_len; t++) {
        for (int i = 0; i < paths.size(); ++i) {
            int x1 = t < paths[i].size() ? paths[i][t].first : paths[i].back().first;
            int y1 = t < paths[i].size() ? paths[i][t].second : paths[i].back().second;
            int x1_next = t + 1 < paths.size() ? paths[i][t + 1].first : paths[i].back().first;
            int y1_next = t + 1 < paths.size() ? paths[i][t + 1].second : paths[i].back().second;
            for (int j = i + 1; j < paths.size(); ++j) {
                int x2 = t < paths[j].size() ? paths[j][t].first : paths[j].back().first;
                int y2 = t < paths[j].size() ? paths[j][t].second : paths[j].back().second;
                int x2_next = t + 1 < paths.size() ? paths[j][t + 1].first : paths[j].back().first;
                int y2_next = t + 1 < paths.size() ? paths[j][t + 1].second : paths[j].back().second;
                if (x1 == x2 && y1 == y2) {
                    return Conflict(i, j, x1, y1, t);
                }
                if (x1_next == x2 && y1_next == y2 && x2_next == x1 && y2_next == y1) {
                    return Conflict(i, j, x1, y1, t + 1);
                }
            }
        }
    }
    return std::nullopt;
}

Path CBS::aStarWithConstrains(int agent, const std::pair<int, int> &start, const std::pair<int, int> &goal,
                              std::vector<Constraint> &constraints) {
    AStar a_star(map.size(), map[0].size(), manhattan);
    auto res_path = a_star.find_path_with_constrains(agent, map, start, goal, constraints);
    return res_path.value();
}

int CBS::computeTotalCost(const std::vector<Path> &paths) {
    int total = 0;
    for (auto pairs: paths) {
        total++;
    }
    return total;
}
