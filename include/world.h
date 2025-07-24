#ifndef WORLD_H
#define WORLD_H

#include "lights.h"
#include "sphere.h"
#include "ray.h"
#include "intersection.h"
#include "color.h"

#include <vector>
#include <memory>


class World
{
    public:
    //fields 
    pointLight world_light; 
    std::vector<Shape*> world_objects; 

    //constructor-destructor
    World(); 
    ~World(); 

    //methods
    Color color_at(const Ray& ray) const; 

    //helper functions 
    std::vector<Intersection> intersect(const Ray& ray) const; 
    Color shade_hit(const Computations& comps) const; 
    bool World::is_shadowed(const Point& point) const; 

}; 

#endif