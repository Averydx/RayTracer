#ifndef INTERSECTION_H
#define INTERSECTION_H

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
    Intersection(double t, const Shape* s); 

    //methods 
    void printIntersection(); 
}; 

/*Using the initializer list here to sidestep variable argument list. 
You can use it like -> intersections({&i1,&i2})
*/
std::vector<Intersection*> intersections(std::initializer_list<Intersection*> items);  

Intersection* find_hit(std::vector<Intersection*> list); 

bool comp_intersection(Intersection* a, Intersection* b); 

#endif