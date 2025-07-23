#ifndef SPHERE_H
#define SPHERE_H

#include "point.h"
#include "intersection.h"
#include "shape.h"
#include "matrix.h"
#include "ray.h"
#include "vector.h"
#include "point.h"
#include "materials.h"


class Sphere : public Shape
{
    public:

        //Constructors
        Sphere():Shape(){}
        std::vector<Intersection> intersect(const Ray& r) const override; 

        //methods
        Vector normal_at(const Point& world_point) const override; 

}; 

#endif