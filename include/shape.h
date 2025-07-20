#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include <vector> 
#include "intersection.h"

class Shape
{
    public: 
        virtual ~Shape() = default;
        virtual std::vector<Intersection*> intersect(const Ray& r) const = 0;
 
}; 

#endif