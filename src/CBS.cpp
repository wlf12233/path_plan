//
// Created by seer on 2025/4/14.
//

#include "CBS.h"

CBS::CBS(const std::vector<std::pair<int, int> > &starts, const std::vector<std::pair<int, int> > &goals,
         const std::vector<std::vector<int> > &map)
    : starts(starts),
      goals(goals),
      map(map) {
}

std::optional<std::vector<Path> > CBS::solve() {
    std::priority_queue<CBSNode, std::vector<CBSNode>, std::greater<CBSNode> > pq;
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
}

Path CBS::aStarWithConstrains(int agent, const std::pair<int, int> &start, const std::pair<int, int> &goal,
                              std::vector<Constraint> &constraints) {
}
