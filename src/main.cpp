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
#include <tclap/CmdLine.h>

using namespace std;

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
    copy(vector.begin(), vector.end(), std::ostream_iterator<int>(cout," "));
    cout << endl;
}

int main(int argc, const char * argv[]) {
    
    try {
        TCLAP::CmdLine cmd("Solution of the ZBS problem for the course MI-PPR.2.", ' ', "0.1");

        
        TCLAP::ValueArg<unsigned int> aArg("a", "a", "Přirozené číslo", true, 0, "number", cmd);
        TCLAP::ValueArg<unsigned int> nArg("n", "n", "Počet uzlů grafu G", true, 0, "number", cmd);
        TCLAP::ValueArg<unsigned int> mArg("m", "m", "Počet hran grafu G", false, 0, "number", cmd);
        TCLAP::ValueArg<unsigned int> kArg("k", "k", "Průměrný stupeň uzlu grafu G", false, 0, "number", cmd);
        TCLAP::ValueArg<string> graphArg("g", "graph", "Jednoduchý souvislý neorientovaný neohodnocený graf", false, "graph.txt", "filename", cmd);
        
        cmd.parse( argc, argv );
        
        unsigned int a = aArg.getValue();
        unsigned int n = nArg.getValue();
        string graphPath = graphArg.getValue();
        
        vector<int> combination;
        
        // first combination (1, 2, 3, ...)
        for (unsigned int i = 1; i <= a; i++) {
            combination.insert(combination.end(), i);
        }
        
        vector_println(combination);
        
        while (next_combination(combination, n, a)) {
            vector_println(combination);
            
            // TODO: Make a function to find number of edges.
        }
        
        return 0;
        

    } catch (TCLAP::ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
        return 1;
    }
}
