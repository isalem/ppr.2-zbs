#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
#include <iterator>

#include "matrix.hpp"
#include <tclap/CmdLine.h>

#include <gmpxx.h>
#include "mpi.h"

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

void vector_println(std::vector<int> & vector) {
    copy(vector.begin(), vector.end(), std::ostream_iterator<int>(cout," "));
    cout << endl;
}

int sequential_zbs(vector<int>& combination, vector<int>& combination_difference, const vector<int>& allSet,
        const SquareMatrix<int>& graph) {

    int countEdges = 0;

    for (vector<int>::iterator row = combination.begin(), rowEnd = combination.end(); row < rowEnd; row++) {
        for (vector<int>::iterator col = combination_difference.begin(), colEnd = combination_difference.end();
             col < colEnd; col++) {
            if (graph(*row -1, *col - 1) == 1 ) {
                countEdges++;
            }
        }
    }

    return countEdges;
}

int main(int argc, const char * argv[]) {

    MPI::Init((int&) argc, (char**&) argv);

    string graph_path;
    int a;

    // Parsing arguments
    try {
        TCLAP::CmdLine cmd("Solution of the ZBS problem for the course MI-PPR.2.", ' ', "0.1");

        TCLAP::ValueArg<int> a_arg("a", "a", "Přirozené číslo", true, 0, "number", cmd);
        TCLAP::ValueArg<string> graph_arg("g", "graph", "Jednoduchý souvislý neorientovaný neohodnocený graf",
                false, "graph.txt", "filename", cmd);

        cmd.parse(argc, argv);

        graph_path = graph_arg.getValue();
        a = a_arg.getValue();

    } catch (TCLAP::ArgException &e) {
        MPI::Finalize();
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
        return 1;
    } // End parsing

    SquareMatrix<int> graph(graph_path);

    if (a >= graph.get_order()) {
        throw runtime_error("A is bigger then graph's order");
    }

    vector<int> combination;
    vector<int> all_edges;

    for (int i = 1; i <= graph.get_order(); i++) {
        all_edges.insert(all_edges.end(), i);
    }

    // first combination (1, 2, 3, ...)
    for (int i = 1; i <= a; i++) {
        combination.insert(combination.end(), i);
    }

    int best_count_edges = numeric_limits<int>::max();
    vector<int> best_combination;
    vector<int> best_combination_complement;

    auto start_clock = chrono::high_resolution_clock::now();

    do {
        vector<int> combination_complement((unsigned long) graph.get_order());
        vector<int>::iterator it;

        it = set_difference(all_edges.begin(), all_edges.end(), combination.begin(), combination.end(),
                combination_complement.begin());

        combination_complement.resize((unsigned long) (it - combination_complement.begin()));

        int count_edges = sequential_zbs(combination, combination_complement, all_edges, graph);

        if (count_edges < best_count_edges) {
            best_count_edges = count_edges;
            best_combination = combination;
            best_combination_complement = combination_complement;
        }

    } while (next_combination<int>(combination, graph.get_order(), a));

    auto end_clock = chrono::high_resolution_clock::now();

    double seconds = chrono::duration_cast<chrono::milliseconds>((end_clock - start_clock)).count() / 1000.0;

    cout << "Time: " << seconds << " seconds" << endl;
    cout << "Set A: ";
    vector_println(best_combination);
    cout << "Set N-A: ";
    vector_println(best_combination_complement);

    cout << "Count edges: " << best_count_edges << endl;

    MPI::Finalize();

    return 0;
}
