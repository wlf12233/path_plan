//
// Created by seer on 2025/6/13.
//
#include <iostream>

class Base {
public:
    int m_value{};

    Base(int value) : m_value(value) {
    }

    void foo() { std::cout << "Base::foo" << std::endl; }
    void bar() { std::cout << "Base::bar" << std::endl; }
    void baz() { std::cout << "Base::baz" << std::endl; }
    friend std::ostream& operator<<(std::ostream& os, const Base& b) {
        os<<"Base";
        return os;
    }

    virtual ~Base() = default;
};

class Derived : public Base {
public:
    std::string m_name{};

    Derived(int value, std::string name) : Base(value), m_name(name) {
    }

    void foo() { std::cout << "Derived::foo" << std::endl; }
    void bar() { std::cout << "Derived::bar" << std::endl; }
    void baz() { std::cout << "Derived::baz" << std::endl; }
    friend std::ostream& operator<<(std::ostream os ,const Derived& d) {
        os<<"Derived";
        return os;
    }
};

class Derived2 : public Base {
public:
    std::string m_name{};

    Derived2(int value, std::string name) : Base(value), m_name(name) {
    }

    void foo() { std::cout << "Derived2::foo" << std::endl; }
    void bar() { std::cout << "Derived::bar" << std::endl; }
    void baz() { std::cout << "Derived::baz" << std::endl; }
    friend std::ostream& operator<<(std::ostream os ,const Derived2& d) {
        os<<"Derived2";
        return os;
    }
};

void printName(Base &base) {
    // std::cout<<base.foo();
}

// int main() {
//     Base* base(0);
//     Derived b1(1, "derived1");
//     Derived d = dynamic_cast<Derived*>(base);
//     // d->foo();
//     // printName(b2);
//     return 0;
// }
