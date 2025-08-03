#ifndef INTERSECTION_H
#define INTERSECTION_H
       
#include "ray.h"
#include "point.h"
#include "vector.h"

#include <vector> 
#include <cstdarg>

//Forward include to break circular dependencies
class Shape;

class Intersection
{
    public: 
        double t; 
        const Shape* s; 

        double u = 0; 
        double v = 0; 

    //constructor 
    Intersection(double t, const Shape* s):t(t),s(s) {}
    Intersection(double t, const Shape* s,double u, double v):t(t),s(s),u(u),v(v){}

    //methods 
    void printIntersection(); 
}; 

class Computations
{
    public: 
        double t; 
        const Shape* s; 
        Point point; 
        Point over_point; 
        Point under_point; 
        Vector eyev; 
        Vector normalv; 
        Vector reflectv; 
        bool inside; 
        double n1; 
        double n2; 

        Computations(const Intersection& I, const Ray& r); 
        Computations(const Intersection& I, const Ray& r, const std::vector<Intersection>& xs); 
}; 
/*Using the initializer list here to sidestep variable argument list. 
You can use it like -> intersections({&i1,&i2})
*/
std::vector<Intersection> intersections(std::initializer_list<Intersection> items);  

const Intersection* find_hit(const std::vector<Intersection>& list); 
bool operator==(Intersection const& obj1,Intersection const& obj2); 
bool comp_intersection(Intersection a, Intersection b); 

#endif