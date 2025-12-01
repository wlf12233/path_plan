//
// Created by seer on 2025/7/15.
//
#include <common.h>
#include <iostream>

#include "../include/Timer.h"
using namespace std;

namespace lift {
    constexpr auto OPEN = "OPEN";
};

constexpr int fibonacci(int n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int fib(int n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

auto add(auto a, auto b) {
    return a + b;
}

std::string fun(const std::string &str1, const std::string &str2) {
    return {};
}

void reference(int &v) {
    std::cout << "左值" << std::endl;
}

void reference(int &&v) {
    std::cout << "右值" << std::endl;
}

template<typename T>
void pass(T &&v) {
    std::cout << "普通传参:";
    reference(v); // 始终调用 reference(int&)
}

// int main() {
//     for (int i = 0; i < 2; ++i) {
//         if (i == 1)
//             std::cout << "1" << std::endl;
//         continue;
//         std::cout << "2" << std::endl;
//     }
//
//     std::cout << "传递右值:" << std::endl;
//     pass(1); // 1是右值, 但输出是左值
//
//     std::cout << "传递左值:" << std::endl;
//     int l = 1;
//     pass(l); // l 是左值, 输出左值
//
//     return 0;
//     const char *p = "123456";
//     std::vector<int> buffer;
//     const size_t buffer_size = buffer.size();
//     cout << add(2.1, 3.1) << endl;
//     Timer timer;
//     int n = 30;
//     cout << "fibonacci(" << n << ") = " << fibonacci(n) << endl;
//     cout << "Time taken: " << timer.elapsed() << " seconds" << endl;
//     timer.reset();
//     cout << "fib(" << n << ") = " << fib(n) << endl;
//     cout << "Time taken: " << timer.elapsed() << " seconds" << endl;
// }
