//
// Created by seer on 2025/5/8.
//
#include <common.h>
#include <iostream>

int fibonacci1(int n) {
    static std::vector result{0, 1};
    if (n < static_cast<int>(result.size())) {
        return result.at(n);
    }
    const int value = fibonacci1(n - 1) + fibonacci1(n - 2);
    result.push_back(value);
    return value;
}

int fibonacci2(int n) {
    if (n <= 2) {
        return n;
    }
    return fibonacci2(n - 1) + fibonacci2(n - 2);
}
