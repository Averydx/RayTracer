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

    //constructor 
    Intersection(double t, const Shape* s):t(t),s(s) {}

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
        Vector eyev; 
        Vector normalv; 
        Vector reflectv; 
        bool inside; 

        Computations(const Intersection& I,const Ray& r); 
};

/*Using the initializer list here to sidestep variable argument list. 
You can use it like -> intersections({&i1,&i2})
*/
std::vector<Intersection> intersections(std::initializer_list<Intersection> items);  

const Intersection* find_hit(const std::vector<Intersection>& list); 

bool comp_intersection(Intersection a, Intersection b); 

#endif