#ifndef POINT_H
#define POINT_H

#include "tuple.h"
#include "vector.h"


// This file defines the Point class for representing points in 3D space
// The Point class is a subclass of Tuple and includes methods for point-specific operations
class Point : public Tuple
{
    public: 

        //Constructors
        Point(double x, double y, double z); 
        Point(); 

        //Operators
        bool operator==(Point const& obj) const; 
        Vector operator-(Point const& obj) const; 
        Point operator+(Vector const& obj) const; 

        //methods 
        void printPoint() const; 
}; 

#endif