#ifndef SPHERE_H
#define SPHERE_H

#include "point.h"
#include "intersection.h"
#include "shape.h"


class Sphere : public Shape
{
    public: 
        Point center; 
        double radius; 

        //Constructors
        Sphere(const Point& center, double radius); 
        std::vector<Intersection*> intersect(const Ray& r) const override; 
}; 

#endif