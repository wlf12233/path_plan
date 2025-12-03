//
// Created by seer on 2025/5/14.
//
#include "common.h"

template<typename T>
class Auto_ptr1 {
    T *m_ptr;

public:
    Auto_ptr1(T *ptr): m_ptr(ptr) {
    }

    // Auto_ptr1(Auto_ptr1&rhs)=delete;
    Auto_ptr1(Auto_ptr1 &rhs): m_ptr(rhs.m_ptr) {
        rhs.m_ptr = nullptr;
    }

    Auto_ptr1 &operator=(Auto_ptr1 &rhs) {
        if (this == &rhs) {
            return *this;
        }
        delete m_ptr;
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
        return *this;
    }

    ~Auto_ptr1() {
        delete m_ptr;
    }

    T &operator*() const { return *m_ptr; }
    T *operator->() const { return m_ptr; }
};

class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

Auto_ptr1<Resource> generateResource() {
    Auto_ptr1<Resource> res{new Resource};
    return res;
}

// int main() {
//     Auto_ptr1<Resource> resource(new Resource());
//     Auto_ptr1<Resource> resource2 = resource;
//     return 0;
// Auto_ptr3<Resource> mainres;
// mainres = generateResource();
// }
