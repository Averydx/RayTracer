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

enum class CYL_TYPE
{
    OPEN,
    CLOSED
}; 

class Cylinder: public Shape
{
    public: 
    //Constructors
    Cylinder():Shape(){}
    Cylinder(double _minimum,double _maximum):Shape(),minimum(_minimum),maximum(_maximum){}
    Cylinder(double _minimum,double _maximum,CYL_TYPE _type):Shape(),minimum(_minimum),maximum(_maximum),type(_type){}
    
    //methods
    Vector local_normal_at(const Point& object_point) const override; 
    bool check_cap(const Ray& r, double t) const; 
    std::vector<Intersection> local_intersect(const Ray& r) const override; 
    void intersect_caps(const Ray& r, std::vector<Intersection>& xs) const; 

    //fields 
    double maximum = std::numeric_limits<double>::infinity(); 
    double minimum = -std::numeric_limits<double>::infinity(); 
    CYL_TYPE type = CYL_TYPE::OPEN; 
}; 

Sphere* glass_sphere(); 

#endif