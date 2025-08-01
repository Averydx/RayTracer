#ifndef WORLD_H
#define WORLD_H

#include "lights.h"
#include "shapes.h"
#include "ray.h"
#include "intersection.h"
#include "color.h"
#include "bvh.h"

#include <vector>
#include <memory>

constexpr int MAX_DEPTH = 5; 

class World
{
    public:
    //fields 
    pointLight world_light; 
    std::vector<Shape*> world_objects; 
    BVHNode* bvh; 

    //constructor-destructor
    World(); 
    ~World(); 

    //methods
    Color color_at(const Ray& ray,int remaining = MAX_DEPTH); 
    void empty_objects(); 

    //helper functions 
    std::vector<Intersection> intersect(const Ray& ray); 
    Color shade_hit(const Computations& comps,int remaining = MAX_DEPTH); 
    bool World::is_shadowed(const Point& point); 
    Color reflected_color(const Computations& comps,int remaining = MAX_DEPTH); 
    Color refracted_color(const Computations& comps,int remaining = MAX_DEPTH); 
    double schlick(const Computations& comps); 
}; 

#endif