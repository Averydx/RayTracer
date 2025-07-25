#include "shapes.h"
#include "tools.h"

#include <math.h>

std::vector<Intersection> Sphere::local_intersect(const Ray& r) const
{
    std::vector<Intersection> intersection_list; 

    Vector sphere_to_ray = r.origin - Point(0.,0.,0.); 

    double a = r.direction * r.direction; 
    double b = 2 * (r.direction * sphere_to_ray); 
    double c = (sphere_to_ray * sphere_to_ray) - 1; 

    double discriminant = b*b - 4 * a * c; 

    if(discriminant < 0)
        return intersection_list; 

    intersection_list.push_back(Intersection((-b - sqrt(discriminant))/(2*a),this)); 
    intersection_list.push_back(Intersection((-b + sqrt(discriminant))/(2*a),this));
    
    return intersection_list; 

}

Vector Sphere::local_normal_at(const Point& object_point) const
{

    Vector local_normal = object_point - Point(0.,0.,0.); 
    return local_normal; 

}

Vector Plane::local_normal_at(const Point& object_point) const
{
    return Vector(0,1,0); 
}

std::vector<Intersection> Plane::local_intersect(const Ray& r) const
{
    std::vector<Intersection> intersection_list; 
    if(abs(r.direction.y) < EPSILON)
        return intersection_list; 

    double t = (-r.origin.y/r.direction.y); 
    intersection_list.push_back(Intersection(t,this)); 
    return intersection_list; 
}

