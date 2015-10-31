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

template <typename T>
bool next_combination(std::vector<T> & combination, int n, int a) {
    for (int i = a - 1; i >= 0; --i)
        if (combination[i] < n - a + i + 1) {
            ++combination[i];
            for (int j = i + 1; j < a; ++j)
                combination[j] = combination[j - 1] + 1;
            return true;
        }
    return false;
}

void vector_println(std::vector<unsigned int> & vector) {
    copy(vector.begin(), vector.end(), std::ostream_iterator<int>(cout," "));
    cout << endl;
}

unsigned int sequenceZBS(vector<unsigned int> & combination, vector<unsigned int> & combinationDifference,
                                 const vector<unsigned int> & allSet,
                                 const SquareMatrix<unsigned int> & graph) {

    unsigned int countEdges = 0;

    for (vector<unsigned int>::iterator row = combination.begin(), rowEnd = combination.end(); row < rowEnd; row++) {
        for (vector<unsigned int>::iterator col = combinationDifference.begin(), colEnd = combinationDifference.end();
             col < colEnd; col++) {
            if (graph(*row -1, *col - 1) == 1 ) {
                countEdges++;
            }
        }
    }

    return countEdges;
}

int main(int argc, const char * argv[]) {
    
    try {
        TCLAP::CmdLine cmd("Solution of the ZBS problem for the course MI-PPR.2.", ' ', "0.1");

        
        TCLAP::ValueArg<unsigned int> aArg("a", "a", "Přirozené číslo", true, 0, "number", cmd);
        TCLAP::ValueArg<unsigned int> nArg("n", "n", "Počet uzlů grafu G", true, 0, "number", cmd);
        TCLAP::ValueArg<unsigned int> mArg("m", "m", "Počet hran grafu G", false, 0, "number", cmd);
        TCLAP::ValueArg<unsigned int> kArg("k", "k", "Průměrný stupeň uzlu grafu G", false, 0, "number", cmd);
        TCLAP::ValueArg<string> graphArg("g", "graph", "Jednoduchý souvislý neorientovaný neohodnocený graf",
                                         false, "graph.txt", "filename", cmd);
        
        cmd.parse( argc, argv );
        
        unsigned int a = aArg.getValue();
        unsigned int n = nArg.getValue();
        string graphPath = graphArg.getValue();

        SquareMatrix<unsigned int> graph(n, graphPath);
        
        vector<unsigned int> combination;
        vector<unsigned int> allSet;

        for (unsigned int i = 1; i <= n; i++) {
            allSet.insert(allSet.end(), i);
        }

        // first combination (1, 2, 3, ...)
        for (unsigned int i = 1; i <= a; i++) {
            combination.insert(combination.end(), i);
        }

        unsigned int bestCountEdges = UINT_MAX;
        vector<unsigned int> bestCombination;
        vector<unsigned int> bestСombinationСomplement;

        do {
            vector<unsigned int> combinationСomplement(n);
            vector<unsigned int>::iterator it;

            it = set_difference(allSet.begin(), allSet.end(),
                                combination.begin(), combination.end(),
                                combinationСomplement.begin());

            combinationСomplement.resize(it - combinationСomplement.begin());

            unsigned int countEdges = sequenceZBS(combination, combinationСomplement, allSet, graph);

            if (countEdges < bestCountEdges) {
                bestCountEdges = countEdges;
                bestCombination = combination;
                bestСombinationСomplement = combinationСomplement;
            }

        } while (next_combination<unsigned int>(combination, n, a));

        cout << "Set A: ";
        vector_println(bestCombination);
        cout << "Set N-A: ";
        vector_println(bestСombinationСomplement);

        cout << "Count edges: " << bestCountEdges << endl;

        return 0;
        

    } catch (TCLAP::ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
        return 1;
    }
}
