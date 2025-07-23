#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"
#include "matrix.h"

#include <vector>

class Ray
{
    public: 
        Point origin; 
        Vector direction; 

        //Constructors
        Ray(const Point& origin,const Vector& direction); 

        //methods
        Point position(double t) const; 
        Ray ray_transform(const Matrix& m) const; 

}; 

#endif