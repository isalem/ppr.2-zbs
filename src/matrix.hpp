//
//  SquareMatrix.hpp
//  zsb
//
//  Created by Banschikov Valentin on 24.10.15.
//  Copyright © 2015 Valentin Banshchikov. All rights reserved.
//

#ifndef SquareMatrix_hpp
#define SquareMatrix_hpp

#include <stdio.h>
#include <vector>

template <typename T>
class SquareMatrix {
private:
    std::vector<std::vector<T>> mat;
    int order;
    
public:
    SquareMatrix(std::string mat_file_path);
    SquareMatrix(const SquareMatrix<T> & rhs);
    virtual ~SquareMatrix();
    
    T& operator()(const int & row, const int & col);
    const T& operator()(const int & row, const int & col) const;
    
    int get_order() const;
};

#include "matrix.cpp"

#endif /* SquareMatrix_hpp */
