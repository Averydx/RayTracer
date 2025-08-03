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

bool operator==(Intersection const& obj1,Intersection const& obj2)
{
    if((obj1.s == obj2.s) && (obj1.t == obj2.t))
        return true; 

    return false; 
}

const Intersection* find_hit(const std::vector<Intersection>& list)
{

    const Intersection* hit = nullptr; 
    for(const Intersection& I : list)
    {
        if(I.t > 0.0)
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

Computations::Computations(const Intersection& I, const Ray& r, const std::vector<Intersection>& xs)
{
    std::vector<const Shape*> container; 
    for(Intersection x :xs)
    {
        if(x == I)
        {
            if(container.empty())
                this->n1 = 1.0; 
            else 
                this->n1 = container.back()->mat.refractive_index; 
        }
        int check = scan_container(container, x.s); 
        if(check == -1)
            container.push_back(x.s); 
        else 
            container.erase(container.begin() + check); 

        if(x == I)
        {
            if(container.empty())
                this->n2 = 1.0; 
            else 
                this->n2 = container.back()->mat.refractive_index; 

            break;
        }
    }

    this->t = I.t; 
    this->s = I.s; 

    //precompute some useful values 
    this->point = r.position(this->t); 
    this->eyev = -r.direction;
    this->normalv = this->s->normal_at(this->point,I); 

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
    this->over_point = this->point + this->normalv * BUMP_EPSILON; 
    this->under_point = this->point + (-1* this->normalv * BUMP_EPSILON); 

    this->reflectv = r.direction.reflect_vector(this->normalv); 
}

Computations::Computations(const Intersection& I, const Ray& r)
{
    std::vector<Intersection> xs = {I}; 
    std::vector<const Shape*> container; 
    for(Intersection x :xs)
    {
        if(x == I)
        {
            if(container.empty())
                this->n1 = 1.0; 
            else 
                this->n1 = container.back()->mat.refractive_index; 
        }
        int check = scan_container(container, x.s); 
        if(check == -1)
            container.push_back(x.s); 
        else 
            container.erase(container.begin() + check); 

        if(x == I)
        {
            if(container.empty())
                this->n2 = 1.0; 
            else 
                this->n2 = container.back()->mat.refractive_index; 

            break;
        }
    }

    this->t = I.t; 
    this->s = I.s; 

    //precompute some useful values 
    this->point = r.position(this->t); 
    this->eyev = -r.direction;
    this->normalv = this->s->normal_at(this->point,I); 

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
    this->under_point = this->point + (-1* this->normalv * EPSILON); 

    this->reflectv = r.direction.reflect_vector(this->normalv); 
}

bool comp_intersection(Intersection a, Intersection b) {
    return a.t < b.t;
}
