//
//  matrix.hpp
//  zsb
//
//  Created by Banschikov Valentin on 24.10.15.
//  Copyright © 2015 Valentin Banshchikov. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <stdio.h>
#include <vector>

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> mat;
    unsigned int rows;
    unsigned int cols;
    
public:
    Matrix(unsigned int _rows, unsigned int _cols, const T & initial);
    Matrix(const Matrix<T> & rhs);
    virtual ~Matrix();
    
    T& operator()(const unsigned int & row, const unsigned int & col);
    const T& operator()(const unsigned int & row, const unsigned int & col) const;
    
    unsigned int get_rows() const;
    unsigned int get_cols() const;
};

#include "matrix.cpp"

#endif /* matrix_hpp */
