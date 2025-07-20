#include "ray.h"
#include "point.h"

#include <iostream> 

Ray::Ray(Point& origin, Vector& direction)
{
    this->origin = origin; 
    this->direction = direction; 
}

Point Ray::position(double t)
{
    return this->origin + this->direction * t; 
}


