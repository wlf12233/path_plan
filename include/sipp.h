//
// Created by seer on 2025/4/1.
//

#ifndef SIPP_H
#define SIPP_H

#include "common.h"

using namespace std;

struct SafeInterval {
    double start, end;
};

struct State {
    int x, y;
    double t;
    int intervalIdx;
    double g, h;

    bool operator>(const State &s) const {
        return (g + h) > (s.g + s.h);
    }
};

class SIPP {
public:
    SIPP(int width, int height);

    void addSafeInterval(int x, int y, double start, double end);

    std::vector<std::pair<int, int> > findPath(std::pair<int, int> start, std::pair<int, int> goal);

private:
    int width, height;
    vector<vector<vector<SafeInterval> > > grid;
    vector<pair<int, int> > moves;

    bool isValid(int x, int y);

    int hashState(int x, int y, int idx);

    double heuristic(pair<int, int> a, pair<int, int> b);

    vector<pair<int, int> > reconstructPath(State goal);
};


#endif //SIPP_H
