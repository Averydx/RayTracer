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

        //Operators
        bool operator==(Point const& obj); 
        Vector operator-(Point const& obj); 
}; 

#endif