//
// Created by seer on 2025/9/17.
//
#include <iostream>

template<typename... Args>
void printAll(Args... args) {
    (std::cout << ... << args);
    std::cout << std::endl;
}
int main() {
    printAll(1, 2, 3);
    std::cout << _MSVC_LANG << std::endl;

}
