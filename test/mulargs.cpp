//
// Created by seer on 2025/9/17.
//
#include <iostream>

template<typename... Args>
void printAll(Args... args) {
    (std::cout << ... << args);
    std::cout << std::endl;
}
