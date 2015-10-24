//
//  main.cpp
//  zsb
//
//  Created by Banschikov Valentin on 24.10.15.
//  Copyright © 2015 Valentin Banshchikov. All rights reserved.
//

#include <iostream>
#include <vector>
#include <iterator>

#include "matrix.hpp"

bool next_combination(std::vector<int> & combination, int n, int a) {
    for (int i = a - 1; i >= 0; --i)
        if (combination[i] < n - a + i + 1) {
            ++combination[i];
            for (int j = i + 1; j < a; ++j)
                combination[j] = combination[j - 1] + 1;
            return true;
        }
    return false;
}

void vector_println(std::vector<int> & vector) {
    std::copy(vector.begin(), vector.end(), std::ostream_iterator<int>(std::cout," "));
    std::cout << "\n";
}

int main(int argc, const char * argv[]) {
    
    unsigned int a = 3;
    unsigned int n = 6;
    
    std::vector<int> combination;
    
    // first combination (1, 2, 3, ...)
    for (unsigned int i = 1; i <= a; i++) {
        combination.insert(combination.end(), i);
    }
    
    vector_println(combination);
    
    while (next_combination(combination, n, a)) {
        vector_println(combination);
    }
    
    return 0;
}
