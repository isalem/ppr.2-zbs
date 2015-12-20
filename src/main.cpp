#include <iostream>
#include <vector>
#include <limits>
#include <iterator>
#include <time.h>
#include <chrono>
#include <math.h>

#include "matrix.hpp"
#include <tclap/CmdLine.h>

#include <gmpxx.h>
#include "mpi.h"

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

void decompose_domain(mpz_class domain_size, int world_rank, int world_size,
        mpz_class * subdomain_start, mpz_class * subdomain_size) {
    *subdomain_start = domain_size / world_size * world_rank;
    *subdomain_size = domain_size / world_size;
    if (world_rank == world_size - 1) {
        // Give remainder to last process
        *subdomain_size += domain_size % world_size;
    }
}

mpz_class combinations_count(int n, int k) {
    mpz_class res = 1;
    for (int i = n - k + 1; i <= n; ++i) {
        res *= i;
    }
    for (int i = 2; i <= k; ++i) {
        res /= i;
    }
    return res;
}

void number_to_combination(int n, int a, mpz_class number, vector<int> & combination) {
    int next = 1;

    while (a > 0) {
        mpz_class temp_number = combinations_count(n - 1, a - 1);

        if (number < temp_number) {
            combination.push_back(next);
            a--;
        } else {
            number -= temp_number;
        }

        n--;
        next++;
    }
}

void vector_println(std::vector<int> & vector) {
    copy(vector.begin(), vector.end(), std::ostream_iterator<int>(cout," "));
    cout << endl;
}

int find_min_graph_cut(vector<int>& combination, vector<int>& combination_complement, const SquareMatrix<int>& graph) {

    int countEdges = 0;

    for (vector<int>::iterator row = combination.begin(), rowEnd = combination.end(); row < rowEnd; row++) {
        for (vector<int>::iterator col = combination_complement.begin(), colEnd = combination_complement.end();
             col < colEnd; col++) {
            if (graph(*row -1, *col - 1) == 1 ) {
                countEdges++;
            }
        }
    }

    return countEdges;
}

const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

double standard_deviation(double data[], double n)
{
    double mean=0.0, sum_deviation=0.0;
    int i;
    for(i = 0; i < n; ++i)
    {
        mean += data[i];
    }
    mean = mean / n;
    for(i = 0; i < n; ++i)
        sum_deviation += (data[i] - mean) * (data[i] - mean);
    return sqrt(sum_deviation / n);
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

    int process_rank;
    int world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (process_rank == 0) {
        cout << "[" << currentDateTime() << "] Initiated" << endl;
        cout << "[" << currentDateTime() << "] A = " << a << endl;
        cout << "[" << currentDateTime() << "] N = " << graph.get_order() << endl;
    }

    auto start_clock = chrono::high_resolution_clock::now();

    mpz_class comb_count = combinations_count(graph.get_order(), a);
    mpz_class subdomain_start;
    mpz_class subdomain_size;

    decompose_domain(comb_count, process_rank, world_size, &subdomain_start, &subdomain_size);

    vector<int> all_edges;
    for (int i = 1; i <= graph.get_order(); i++) {
        all_edges.insert(all_edges.end(), i);
    }

    vector<int> start_combination;
    number_to_combination(graph.get_order(), a, subdomain_start, start_combination);
    vector<int> stop_combination;
    number_to_combination(graph.get_order(), a, subdomain_start + subdomain_size - 1, stop_combination);

    int best_count_edges = numeric_limits<int>::max();
    vector<int> best_combination;
    vector<int> best_combination_complement;
    const unsigned long combination_length = all_edges.size() - start_combination.size();

    do {
        vector<int> combination_complement(combination_length);
        set_difference(all_edges.begin(), all_edges.end(), start_combination.begin(), start_combination.end(),
                combination_complement.begin());

        int count_edges = find_min_graph_cut(start_combination, combination_complement, graph);

        if (count_edges < best_count_edges) {
            best_count_edges = count_edges;
            best_combination = start_combination;
            best_combination_complement = combination_complement;
        }

        if (start_combination == stop_combination) {
            break;
        }

    } while (next_combination(start_combination, graph.get_order(), a));

    auto end_clock = chrono::high_resolution_clock::now();
    cout << "[" << currentDateTime() << "] Process " << process_rank  << " finished" << endl;

    MPI_Barrier(MPI_COMM_WORLD);

    int global_best_count_edges;
    MPI_Reduce(&best_count_edges, &global_best_count_edges, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
        cout << "[" << currentDateTime() << "] Count edges: " << global_best_count_edges << endl;
    }

    double time = chrono::duration_cast<chrono::milliseconds>((end_clock - start_clock)).count() / 1000.0;

    double global_time;
    MPI_Reduce(&time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    double * local_times = NULL;
    if (process_rank == 0) {
        local_times = (double*) malloc(sizeof(double) * world_size);
    }
    MPI_Gather(&time, 1, MPI_DOUBLE, local_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
        double std_der = standard_deviation(local_times, world_size);

        cout << "[" << currentDateTime() << "] Time: " << global_time << endl;
        cout << "[" << currentDateTime() << "] Deviation: " << std_der << endl;

        free(local_times);
    }

    MPI::Finalize();

    return 0;
}
