//
// Created by seer on 2025/7/31.
//
#include <iostream>

template<typename Derived>
class CBase {
public:
    void commonMethod() {
        static_cast<Derived *>(this)->implementation();
    }

    void anotherCommonMethod() {
        std::cout << "Another common method in Base" << std::endl;
    }
};

class Derived1 : public CBase<Derived1> {
public:
    void implementation() {
        std::cout << "Implementation of Derived1" << std::endl;
    }
};

class Derived2 : public CBase<Derived2> {
public:
    void implementation() {
        std::cout << "Implementation of Derived2" << std::endl;
    }
};

// int main() {
//     Derived1 d1;
//     d1.commonMethod();
//     d1.anotherCommonMethod();
//
//     Derived2 d2;
//     d2.implementation();
//     d2.anotherCommonMethod();
// }
