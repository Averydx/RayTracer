#ifndef INTERSECTION_H
#define INTERSECTION_H
       
#include "ray.h"
#include "point.h"
#include "vector.h"

#include <vector> 
#include <cstdarg>

//Forward include to break circular dependencies
class Shape;

// This file defines the Intersection class and Computations class for ray tracing
// The Intersection class represents an intersection between a ray and a shape
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

// This class holds the results of an intersection computation
// It includes the intersection time, the shape hit, the point of intersection, and other relevant data
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
// Function to create a vector of intersections from an initializer list
std::vector<Intersection> intersections(std::initializer_list<Intersection> items);  

// Function to find the closest intersection in a list of intersections
// Returns a pointer to the closest intersection or nullptr if no valid intersection is found
const Intersection* find_hit(const std::vector<Intersection>& list); 

//equality operator for Intersection objects
// Returns true if the first intersection's shape and time match the second's
bool operator==(Intersection const& obj1,Intersection const& obj2); 

// Comparison operator for Intersection objects
// Returns true if the first intersection's time is less than the second's
bool comp_intersection(Intersection a, Intersection b); 

#endif