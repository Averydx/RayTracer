#include "intersection.h"

Intersection::Intersection(double t, const Shape* s)
{
    this->t = t; 
    this->s = s; 
} 

std::vector<Intersection*> intersections(std::initializer_list<Intersection*> items)
{
    return std::vector<Intersection*>(items);
}
