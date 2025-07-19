#ifndef MATRIX_H
#define MATRIX_H

#include <vector> 

#include "tuple.h"
#include "vector.h"
#include "point.h"
#include "color.h"

class Matrix
{

   public: 

    //fields
    int rows; 
    int cols; 
    std::vector<double> data; 

    //operators 
    bool operator==(Matrix const& obj); 
    Matrix operator*(Matrix const& obj); 
    Color operator*(Color const& obj); 
    Vector operator*(Vector const& obj); 
    Point operator*(Point const& obj); 

    //Constructors
    Matrix(int rows,int cols); 

    //methods
    double getElement(int row, int col) const;
    void setElement(int row,int col, double value);  
    void setAllElements(std::vector<double>& data); 
    void setIdentity(); 
}; 

#endif