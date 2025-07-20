#ifndef POINT_H
#define POINT_H

#include "tuple.h"
#include "vector.h"

/*
* Defines a point. This is a subclass of tuple designed to represent a point in space. 
*
*/

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