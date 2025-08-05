#include "ray.h"
#include "point.h"

#include <iostream> 

// This class represents a ray in 3D space with an origin and a direction.
Ray::Ray(const Point& origin, const Vector& direction)
{
    this->origin = origin; 
    this->direction = direction; 
}

// This function calculates the position of a point along the ray at a given parameter t.
// It returns a Point that represents the position at distance t from the ray's origin in the direction of the ray.
Point Ray::position(double t) const
{
    return this->origin + this->direction * t; 
}

// This function transforms the ray using a transformation matrix.
// It applies the transformation to both the origin and the direction of the ray.
// The transformed ray is returned as a new Ray object.
Ray Ray::ray_transform(const Matrix& m) const
{
    Ray r(this->origin,this->direction); 
    r.origin = m * r.origin; 
    r.direction = m * r.direction; 

    return r; 
}


