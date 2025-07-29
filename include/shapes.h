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

#include <array>


class Sphere : public Shape
{
    public:

        //Constructors
        Sphere():Shape(){}
        std::vector<Intersection> local_intersect(const Ray& r) const override; 
        
        //methods
        Vector local_normal_at(const Point& object_point) const override; 

}; 

class Plane : public Shape
{
    public:
        //Constructors
        Plane():Shape(){}
        std::vector<Intersection> local_intersect(const Ray& r) const override; 
        
        //methods
        Vector local_normal_at(const Point& object_point) const override; 
};

class Cube : public Shape 
{
    public: 
    //Constructors
    Cube():Shape(){}
    std::vector<Intersection> local_intersect(const Ray& r) const override; 
    
    //methods
    Vector local_normal_at(const Point& object_point) const override; 
    std::array<double,2> check_axis(double origin, double direction) const; 
}; 

Sphere* glass_sphere(); 

#endif