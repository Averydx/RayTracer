#include "ray.h"
#include "point.h"

#include <iostream> 

Ray::Ray(const Point& origin, const Vector& direction)
{
    this->origin = origin; 
    this->direction = direction; 
}

Point Ray::position(double t) const
{
    return this->origin + this->direction * t; 
}

Ray Ray::ray_transform(const Matrix& m) const
{
    Ray r(this->origin,this->direction); 
    r.origin = m * r.origin; 
    r.direction = m * r.direction; 

    return r; 
}


