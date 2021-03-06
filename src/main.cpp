#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
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

unsigned int sequentialZBS(vector<unsigned int> & combination, vector<unsigned int> & combinationDifference,
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
        TCLAP::ValueArg<string> graphArg("g", "graph", "Jednoduchý souvislý neorientovaný neohodnocený graf",
                                         false, "graph.txt", "filename", cmd);
        
        cmd.parse( argc, argv );

        auto start_clock = chrono::high_resolution_clock::now();

        string graphPath = graphArg.getValue();
        SquareMatrix<unsigned int> graph(graphPath);

        unsigned int a = aArg.getValue();

        if (a >= graph.get_order()) {
            throw runtime_error("A is bigger then graph's order");
        }

        vector<unsigned int> combination;
        vector<unsigned int> allSet;

        for (unsigned int i = 1; i <= graph.get_order(); i++) {
            allSet.insert(allSet.end(), i);
        }

        // first combination (1, 2, 3, ...)
        for (unsigned int i = 1; i <= a; i++) {
            combination.insert(combination.end(), i);
        }

        unsigned int bestCountEdges = numeric_limits<unsigned int>::max();
        vector<unsigned int> bestCombination;
        vector<unsigned int> bestCombinationComplement;

        do {
            vector<unsigned int> combinationComplement(graph.get_order());
            vector<unsigned int>::iterator it;

            it = set_difference(allSet.begin(), allSet.end(),
                                combination.begin(), combination.end(),
                                combinationComplement.begin());

            combinationComplement.resize((unsigned long) (it - combinationComplement.begin()));

            unsigned int countEdges = sequentialZBS(combination, combinationComplement, allSet, graph);

            if (countEdges < bestCountEdges) {
                bestCountEdges = countEdges;
                bestCombination = combination;
                bestCombinationComplement = combinationComplement;
            }

        } while (next_combination<unsigned int>(combination, graph.get_order(), a));

        auto end_clock = chrono::high_resolution_clock::now();

        double seconds = chrono::duration_cast<chrono::milliseconds>((end_clock - start_clock)).count() / 1000.0;

        cout << "Time: " << seconds << " seconds" << endl;
        cout << "Set A: ";
        vector_println(bestCombination);
        cout << "Set N-A: ";
        vector_println(bestCombinationComplement);

        cout << "Count edges: " << bestCountEdges << endl;

        return 0;
        

    } catch (TCLAP::ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
        return 1;
    }
}
