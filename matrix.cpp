//
//  matrix.cpp
//  zsb
//
//  Created by Banschikov Valentin on 24.10.15.
//  Copyright © 2015 Valentin Banshchikov. All rights reserved.
//

#ifndef matrix_cpp
#define matrix_cpp

#include "matrix.hpp"

template <typename T>
Matrix<T>::Matrix(unsigned int _rows, unsigned int _cols, const T & _initial) {
    mat.resize(_rows);
    for (unsigned int i = 0; i < mat.size(); i++) {
        mat[i].resize(_cols, _initial);
    }
    rows = _rows;
    cols = _cols;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> & rhs) {
    mat = rhs.mat;
    cols = rhs.get_cols();
    rows = rhs.get_rows();
}

template <typename T>
Matrix<T>::~Matrix<T>() { }

template<typename T>
T& Matrix<T>::operator()(const unsigned int & row, const unsigned int & col) {
    return this->mat[row][col];
}

template<typename T>
const T& Matrix<T>::operator()(const unsigned int & row, const unsigned int & col) const {
    return this->mat[row][col];
}

template <typename T>
unsigned int Matrix<T>::get_rows() const {
    return this->rows;
}

template <typename T>
unsigned int Matrix<T>::get_cols() const {
    return this->cols;
}

#endif