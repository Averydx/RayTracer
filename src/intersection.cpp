#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "tools.h"

#include <algorithm>
#include <iostream>

void Intersection::printIntersection()
{
    std::cout<<"Intersection with time: "<<this->t<<std::endl<<"at Shape ID: "<<this->s<<std::endl; 
}

std::vector<Intersection> intersections(std::initializer_list<Intersection> items)
{
    return std::vector<Intersection>(items);
}

const Intersection* find_hit(const std::vector<Intersection>& list)
{

    const Intersection* hit = nullptr; 
    for(const Intersection& I : list)
    {
        if(I.t > 0)
        {
            if(hit == nullptr)
            {
                hit = &I; 
            }

            else if(I.t < hit->t)
            {
                hit = &I; 
            }
        }

    }

    return hit;
}

Computations::Computations(const Intersection& I, const Ray& r)
{
    this->t = I.t; 
    this->s = I.s; 

    //precompute some useful values 
    this->point = r.position(this->t); 
    this->eyev = -r.direction;
    this->normalv = this->s->normal_at(this->point); 

    if((this->normalv * this->eyev) < 0)
    {
        this->inside = true; 
        this->normalv = -this->normalv; 
    }
    else 
    {
        this->inside = false; 
    }

    //Make sure to do this after checking whether the normal vector needs to be negated. 
    this->over_point = this->point + this->normalv * EPSILON; 
    this->reflectv = r.direction.reflect_vector(this->normalv); 
}

bool comp_intersection(Intersection a, Intersection b) {
    return a.t < b.t;
}
