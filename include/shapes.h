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
#include "bvh.h"

#include <array>


class Sphere : public Shape
{
    public:

        //Constructors
        Sphere():Shape(){}
        std::vector<Intersection> local_intersect(const Ray& r) const override; 
        
        //methods
        Vector local_normal_at(const Point& object_point) const override; 
        AABB bounds() const override; 

}; 

class Group: public Shape
{
    public:
    //Constructors
    Group():Shape(){isGroup = true;}
    
    //Need destructor
    ~Group(); 

    std::vector<Intersection> local_intersect(const Ray& r) const override; 
    
    //methods
    Vector local_normal_at(const Point& object_point) const override; 
    AABB bounds() const;
    void add_child(Shape* s); 

    //fields
    std::vector<Shape*> children; 

}; 

class Plane : public Shape
{
    public:
        //Constructors
        Plane():Shape(){}
        Plane(double z_min,double z_max,double x_min,double x_max):Shape(),z_maximum(z_max),z_minimum(z_min),x_maximum(x_max),x_minimum(x_min){}
        std::vector<Intersection> local_intersect(const Ray& r) const override; 
        
        //methods
        Vector local_normal_at(const Point& object_point) const override; 
        AABB bounds() const; 

        //fields
        double x_minimum = -100; 
        double x_maximum = 100; 

        double z_minimum = -100; 
        double z_maximum = 100; 

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
    AABB bounds() const; 
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
    AABB bounds() const; 

    //fields 
    double maximum = 100; 
    double minimum = -100; 
    CYL_TYPE type = CYL_TYPE::OPEN; 
}; 

Sphere* glass_sphere(); 

#endif