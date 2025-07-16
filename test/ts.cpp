//
// Created by seer on 2025/7/15.
//
#include <iostream>
using namespace std;
namespace lift {
    constexpr auto OPEN = "OPEN";
};
int main() {

    string status = "OPEN";
    if (status == lift::OPEN) {
        cout << "Opening" << endl;
    }
}
