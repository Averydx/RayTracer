#include "sphere.h"

Sphere::Sphere(const Point& center,double radius):Shape()
{
    this->center = center; 
    this->radius = radius; 
    
}

std::vector<Intersection*> Sphere::intersect(const Ray& r) const
{

    std::vector<Intersection*> intersections; 

    Vector sphere_to_ray = r.origin - this->center; 

    double a = r.direction * r.direction; 
    double b = 2 * (r.direction * sphere_to_ray); 
    double c = (sphere_to_ray * sphere_to_ray) - this->radius * this->radius; 

    double discriminant = b*b - 4 * a * c; 

    if(discriminant < 0)
        return intersections; 

    intersections.push_back(new Intersection((-b - sqrt(discriminant))/(2*a),this)); 
    intersections.push_back(new Intersection((-b + sqrt(discriminant))/(2*a),this));
    
    return intersections; 

}