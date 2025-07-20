#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"

#include <vector>

class Ray
{
    public: 
        Point origin; 
        Vector direction; 

        //Constructors
        Ray(Point& origin,Vector& direction); 

        //methods
        Point position(double t); 

}; 

#endif