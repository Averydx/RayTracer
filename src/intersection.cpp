#include "intersection.h"
#include <algorithm>
#include <iostream>

Intersection::Intersection(double t, const Shape* s)
{
    this->t = t; 
    this->s = s; 
} 

void Intersection::printIntersection()
{
    std::cout<<"Intersection with time: "<<this->t<<std::endl<<"at Shape ID: "<<this->s<<std::endl; 
}

std::vector<Intersection*> intersections(std::initializer_list<Intersection*> items)
{
    return std::vector<Intersection*>(items);
}

Intersection* find_hit(std::vector<Intersection*> list)
{

    Intersection* hit = nullptr; 
    for(Intersection* I : list)
    {
        if(I!= nullptr && I->t > 0)
        {
            if(hit == nullptr)
            {
                hit = I; 
            }

            else if(I->t < hit->t)
            {
                hit = I; 
            }
        }

    }

    return hit;
}

bool comp_intersection(Intersection* a, Intersection* b) {
    return a->t < b->t;
}
