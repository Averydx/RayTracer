#include "world.h"
#include "transformations.h"
#include "lights.h"

#include <algorithm>


World::World()
{
    this->world_light.position = Point(-10,10,-10); 
    this->world_objects.push_back(new Sphere()); 
    this->world_objects.push_back(new Sphere()); 

    this->world_objects[0]->setMaterial(Material(0.1,0.7,0.2,200.,Color(0.8,1.0,0.6))); 
    this->world_objects[1]->setTransform(scaling(0.5,0.5,0.5)); 
}
World::~World()
{
    for(Shape* s: this->world_objects)
    {
        delete s; 
    }
}

std::vector<Intersection> World::intersect(const Ray& ray) const
{
    std::vector<Intersection> intersection_list; 
    intersection_list.reserve(world_objects.size() * 2); 
    for(Shape* s: this->world_objects)
    {
        std::vector<Intersection> temp_list = s->intersect(ray); 
        intersection_list.insert(intersection_list.end(),temp_list.begin(),temp_list.end()); 
    }
    std::sort(intersection_list.begin(),intersection_list.end(),comp_intersection); 

    return intersection_list; 
}

Color World::shade_hit(const Computations& comps) const
{
    bool in_shadow = this->is_shadowed(comps.over_point); 
    return lighting(comps.s->mat,this->world_light,comps.over_point,comps.eyev,comps.normalv,in_shadow); 
}

Color World::color_at(const Ray& ray) const
{
    std::vector<Intersection> _ints = this->intersect(ray); 
    const Intersection* hit = find_hit(_ints); 

    if(hit == nullptr)
        return Color(0.f,0.f,0.f); 

    Computations comps(*hit,ray);
    
    return shade_hit(comps); 
}

bool World::is_shadowed(const Point& point) const
{
    Vector shadow_vec = this->world_light.position - point; 
    double distance = shadow_vec.magnitude(); 
    Ray shadow_ray(point,shadow_vec.normalize()); 

    std::vector<Intersection> inters = this->intersect(shadow_ray); 
    const Intersection* hit = find_hit(inters); 

    if(hit != nullptr && hit->t < distance)
        return true; 

    return false; 
}