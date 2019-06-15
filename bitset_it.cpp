
#include "bitset_it.hpp"

#include <bitset>
#include <iostream>

int main() {
    std::bitset<10> a{333};
    
    // Iterate over all values
    for( auto v : a ) {
    	std::cout << v << " "; // prints 0 2 3 6 8
    }
    
    // Num bits set (works but inefficient)
    auto num_set = std::distance( std::begin(a), std::end(a) );
    
    return 0;
}
