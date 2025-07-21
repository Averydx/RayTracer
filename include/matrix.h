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
    Matrix operator*(Matrix const& obj) const; 
    Color operator*(Color const& obj) const; 
    Vector operator*(Vector const& obj) const; 
    Point operator*(Point const& obj) const; 

    //Constructors
    Matrix(int rows,int cols); 
    Matrix(int rows, int cols, std::vector<double>& data); 

    //methods
    double getElement(int row, int col) const;
    void setElement(int row,int col, double value);  
    void setAllElements(std::vector<double>& data); 
    void setIdentity();
    void printMatrix(); 
    void transpose();  
    double determinant() const; 
    double minor(int rowToRemove,int colToRemove) const; 
    double cofactor(int rowToRemove,int colToRemove) const; 
    Matrix subMatrix(int rowToRemove, int colToRemove) const; 
    Matrix inverse() const; 
}; 

bool operator==(Matrix const& obj1,Matrix const& obj2); 

#endif