//
// Created by seer on 2025/5/8.
//

#ifndef TIMMER_H
#define TIMMER_H
#include <chrono>

class Timer {
private:
    using Clock = std::chrono::steady_clock;
    using Seconds = std::chrono::duration<double>;

public:
    std::chrono::time_point<Clock> start{Clock::now()};

    void reset() {
        start = Clock::now();
    }

    double elapsed() const {
        return std::chrono::duration_cast<Seconds>(Clock::now() - start).count();
    }
};


#endif //TIMMER_H
