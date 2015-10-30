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

#include "matrix.hpp"

using namespace std;

template <typename T>
SquareMatrix<T>::SquareMatrix(unsigned int _order, const T & _initial) {
    mat.resize(_order);
    for (unsigned int i = 0; i < mat.size(); i++) {
        mat[i].resize(_order, _initial);
    }
    order = _order;
}

template <typename T>
SquareMatrix<T>::SquareMatrix(unsigned int _order, string matFilePath) {
    ifstream matFile(matFilePath);

    if (!matFile) {
        throw runtime_error("Can't open file " + matFilePath);
    }

    mat.resize(_order);

    bool matrixOrderLine = true;
    unsigned int row = 0;

    for (string line; getline(matFile, line);) {
        if (matrixOrderLine) {
            int orderFromFile = stoi(line);

            if (orderFromFile != _order) {
                throw runtime_error("Matrix order error");
            }

            matrixOrderLine = false;
        } else {
            mat[row].resize(_order);
            unsigned int col = 0;

            for (string::iterator it = line.begin(), itEnd = line.end(); it < itEnd; it++) {
                unsigned int number = *it - '0';
                mat[row][col] = number;
                col++;
            }

            row++;
        }
    }

    matFile.close();

    order = _order;
}

template <typename T>
SquareMatrix<T>::SquareMatrix(const SquareMatrix<T> & rhs) {
    mat = rhs.mat;
    order = rhs.get_order();
}

template <typename T>
SquareMatrix<T>::~SquareMatrix<T>() { }

template<typename T>
T& SquareMatrix<T>::operator()(const unsigned int & row, const unsigned int & col) {
    return this->mat[row][col];
}

template<typename T>
const T& SquareMatrix<T>::operator()(const unsigned int & row, const unsigned int & col) const {
    return this->mat[row][col];
}

template <typename T>
unsigned int SquareMatrix<T>::get_order() const {
    return this->order;
}

#endif