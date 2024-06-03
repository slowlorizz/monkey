#include "monkey.hpp"
#include <iostream>

using namespace monkey;

void print(List<int>::Iterator* it) {
    std::cout << *it->v << "  ";
}

int mp(List<int>::Iterator* it) {
    return *it->v * 3;
}

int main() {
    List<int> ls;

    for (int i = 0; i < 20; ++i) {
        if ((i % 2) == 0) {
            ls.push(i);
        }
        else {
            ls.push_front(i);
        }
    }

    ls.map(&mp);
    ls.foreach(&print);

    return 0;
}