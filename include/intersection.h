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
    Intersection::Intersection(double t, const Shape* s); 
}; 

/*Using the initializer list here to sidestep variable argument list. 
You can use it like -> intersections({&i1,&i2})
*/
std::vector<Intersection*> intersections(std::initializer_list<Intersection*> items);  


#endif