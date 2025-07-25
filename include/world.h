#ifndef WORLD_H
#define WORLD_H

#include "lights.h"
#include "shapes.h"
#include "ray.h"
#include "intersection.h"
#include "color.h"

#include <vector>
#include <memory>

constexpr int MAX_DEPTH = 5; 

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
    Color color_at(const Ray& ray,int remaining = MAX_DEPTH) const; 

    //helper functions 
    std::vector<Intersection> intersect(const Ray& ray) const; 
    Color shade_hit(const Computations& comps,int remaining = MAX_DEPTH) const; 
    bool World::is_shadowed(const Point& point) const; 
    Color reflected_color(const Computations& comps,int remaining = MAX_DEPTH) const; 

}; 

#endif