
#include "bitset_it.hpp"

#include <bitset>
#include <iostream>

int main() {
    std::bitset<64> a(333);
    for( auto v : a ) {
    	std::cout << v << " "; // prints 0 2 3 6 8
    }
}
