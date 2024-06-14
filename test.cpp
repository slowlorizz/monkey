#include "monkey.hpp"
#include <iostream>

using namespace monkey;


int main() {
    ArrayList<int> lst;

    std::cout << "\n------------\n" << std::endl;
    
    for(int i = 0; i < 35; ++i) {
        lst.push(i);
        std::cout << "Push: " << i << std::endl;
    }

    std::cout << "\n------------\n" << std::endl;

    int pv = lst.pop(2);

    std::cout << "Popped:   " << pv << std::endl;
    std::cout << "Capacity: " << lst.cap() << std::endl;
    std::cout << "Length:   " << lst.len() << std::endl;

    std::cout << std::endl;

    for (int i = 0; i < lst.len(); ++i) {
        std::cout << lst[i] << " ";
    }

    std::cout << std::endl;

    return 0;
}