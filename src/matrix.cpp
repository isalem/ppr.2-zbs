//
//  SquareMatrix.cpp
//  zsb
//
//  Created by Banschikov Valentin on 24.10.15.
//  Copyright © 2015 Valentin Banshchikov. All rights reserved.
//

#ifndef SquareMatrix_cpp
#define SquareMatrix_cpp

#include <ostream>
#include <fstream>
#include <stdexcept>

#include "matrix.hpp"

using namespace std;

template <typename T>
SquareMatrix<T>::SquareMatrix(string mat_file_path) {
    ifstream mat_file(mat_file_path);

    if (!mat_file) {
        throw runtime_error("Can't open file " +mat_file_path);
    }

    bool matrixOrderLine = true;
    int row = 0;

    for (string line; getline(mat_file, line);) {
        if (matrixOrderLine) {
            this->order = stoi(line);

            mat.resize((unsigned long) this->order);

            matrixOrderLine = false;
        } else {
            mat[row].resize((unsigned long) this->order);
            int col = 0;

            for (string::iterator it = line.begin(), itEnd = line.end(); it < itEnd; it++) {
                int number = *it - '0';
                mat[row][col] = number;
                col++;
            }

            row++;
        }
    }

    mat_file.close();
}

template <typename T>
SquareMatrix<T>::SquareMatrix(const SquareMatrix<T> & rhs) {
    mat = rhs.mat;
    order = rhs.get_order();
}

template <typename T>
SquareMatrix<T>::~SquareMatrix<T>() { }

template<typename T>
T& SquareMatrix<T>::operator()(const int & row, const int & col) {
    return this->mat[row][col];
}

template<typename T>
const T& SquareMatrix<T>::operator()(const int & row, const int & col) const {
    return this->mat[row][col];
}

template <typename T>
int SquareMatrix<T>::get_order() const {
    return this->order;
}

#endif