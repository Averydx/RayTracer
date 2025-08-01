#include "world.h"
#include "transformations.h"
#include "lights.h"
#include "tools.h"

#include <algorithm>
#include <iostream>


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

std::vector<Intersection> World::intersect(const Ray& ray)
{
    //std::vector<Intersection> intersection_list = bvh_intersect(this->bvh,ray); 
    std::vector<Intersection> intersection_list = {}; 
    for(Shape* s: this->world_objects)
    {
        std::vector<Intersection> temp_list = s->intersect(ray); 
        intersection_list.insert(intersection_list.end(),temp_list.begin(),temp_list.end()); 
    }
    std::sort(intersection_list.begin(),intersection_list.end(),comp_intersection); 

    return intersection_list; 
}

Color World::shade_hit(const Computations& comps,int remaining)
{
    bool in_shadow = this->is_shadowed(comps.over_point); 

    Color surface =  lighting(comps.s->mat,comps.s,this->world_light,comps.over_point,comps.eyev,comps.normalv,in_shadow);

    Color reflected = Color(0,0,0); 
    if(comps.s->mat.reflective > 0.0)
        reflected = this->reflected_color(comps,remaining); 

    Color refracted = Color(0,0,0); 
    if(comps.s->mat.transparency > 0.0)
        refracted = this->refracted_color(comps,remaining); 

    if(comps.s->mat.reflective > 0.0 && comps.s->mat.transparency > 0.0)
    {
        double reflectance = this->schlick(comps); 
        return surface + reflected * reflectance + refracted * (1.0 - reflectance); 
    }
        return surface + reflected + refracted;    
}

Color World::color_at(const Ray& ray,int remaining)
{
    std::vector<Intersection> _ints = this->intersect(ray); 
    const Intersection* hit = find_hit(_ints); 

    if(hit == nullptr)
        return Color(0.0,0.0,0.0); 

    Computations comps(*hit,ray,_ints);
    
    return shade_hit(comps,remaining); 
}

bool World::is_shadowed(const Point& point)
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

Color World::reflected_color(const Computations& comps,int remaining)
{
    if(remaining <= 1)
        return Color(0.0,0.0,0.0); 

    Ray reflect_ray(comps.over_point,comps.reflectv); 
    Color color = this->color_at(reflect_ray,remaining-1); 

    return color * comps.s->mat.reflective; 
}

void World::empty_objects()
{
    for(Shape* s: this->world_objects)
    {
        delete s; 
    }
    this->world_objects.clear(); 
}

Color World::refracted_color(const Computations& comps,int remaining)
{
    if(remaining == 0)
        return Color(0,0,0); 

    //Find the ratio of first index of refraction to the second
    double n_ratio = comps.n1 / comps.n2; 

    //cos(theta_i) is the dot product of the two vectors
    double cos_i = comps.eyev * comps.normalv; 

    //Trig identity
    double sin2_t = pow(n_ratio,2) * (1-pow(cos_i,2));

    //total internal reflection 
    if(sin2_t > 1.0)
        return Color(0,0,0); 

    double cos_t = sqrt(1 - sin2_t); 

    //direction of refracted ray
    //Vector direction = comps.normalv * (n_ratio * cos_i - cos_t) - comps.eyev * n_ratio; 
    Vector direction =  (n_ratio * cos_i - cos_t) * comps.normalv - n_ratio * comps.eyev; 
    Ray refract_ray(comps.under_point,direction); 

    Color color = this->color_at(refract_ray,remaining-1) * comps.s->mat.transparency; 

    return color; 
}

double World::schlick(const Computations& comps)
{
    double _cos = comps.eyev * comps.normalv;

    //total internal reflection occurs if n1 > n2
    if(comps.n1 > comps.n2)
    {
        double n = comps.n1 / comps.n2; 
        double sin2_t = n*n * (1.0 - _cos*_cos); 
        if(sin2_t > 1)
            return 1.0; 

        //compute cosine of theta_t using trig identity
        double cos_t = sqrt(1 - sin2_t); 

        //when n1 > n2 use cos(theta_t) instead
        _cos = cos_t; 
    }

    double r0 = pow(((comps.n1 - comps.n2)/(comps.n1 + comps.n2)),2); 
    return r0 + (1-r0) * pow(1-_cos,5);  
}

