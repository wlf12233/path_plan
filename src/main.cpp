//
// Created by seer on 2025/12/1.
//
#include <iostream>


int main(int argc, char *argv[]) {
    float x = 1.0;
    short c = *(short*)&x;
    std::cout << c << std::endl;

    short d = 1;
    float y = *(float*)&d;
    *(float*)&d = 2;
    std::cout << y << std::endl;
}
