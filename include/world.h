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

// This file defines the World class for managing the scene in a ray tracing application
// It includes methods for adding shapes, intersecting rays with the world, and calculating colors at intersections
// The World class also handles lighting and shading calculations for the shapes in the scene
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
    Color color_at(const Ray& ray,int remaining = MAX_DEPTH); // Calculates the color at a given ray, considering intersections and lighting 
    void empty_objects(); // Clears all shapes from the world
    void add_object(Shape* s); // Adds a shape to the world

    //helper functions 
    std::vector<Intersection> intersect(const Ray& ray); // Intersects a ray with the world, returning a list of intersections
    Color shade_hit(const Computations& comps,int remaining = MAX_DEPTH); // Shades the hit point based on the material properties and lighting
    bool World::is_shadowed(const Point& point); // Checks if a point is in shadow with respect to the light source
    Color reflected_color(const Computations& comps,int remaining = MAX_DEPTH); // Calculates the color of the reflected ray at the intersection point
    Color refracted_color(const Computations& comps,int remaining = MAX_DEPTH); // Calculates the color of the refracted ray at the intersection point
    double schlick(const Computations& comps); // Calculates the Schlick approximation for reflectance
}; 

#endif