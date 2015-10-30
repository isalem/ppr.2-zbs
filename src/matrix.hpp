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
    unsigned int order;
    
public:
    SquareMatrix(unsigned int _order, const T & initial);
    SquareMatrix(unsigned int _order, std::string matFilePath);
    SquareMatrix(const SquareMatrix<T> & rhs);
    virtual ~SquareMatrix();
    
    T& operator()(const unsigned int & row, const unsigned int & col);
    const T& operator()(const unsigned int & row, const unsigned int & col) const;
    
    unsigned int get_order() const;
};

#include "matrix.cpp"

#endif /* SquareMatrix_hpp */
