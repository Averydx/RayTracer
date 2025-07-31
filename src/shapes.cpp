#include "shapes.h"
#include "tools.h"

#include <math.h>
#include <algorithm> 

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

Vector Cube::local_normal_at(const Point& object_point) const
{
    double maxc = std::max(std::max(abs(object_point.x),abs(object_point.y)),abs(object_point.z)); 
    if(maxc == abs(object_point.x))
        return Vector(object_point.x,0,0); 
    else if(maxc == abs(object_point.y))
        return Vector(0,object_point.y,0); 
    
    return Vector(0,0,object_point.z); 
}

std::array<double,2> Cube::check_axis(double origin, double direction) const
{
    double tmin_numerator = (-1 - origin); 
    double tmax_numerator = (1 - origin); 

    double tmin = tmin_numerator / direction; 
    double tmax = tmax_numerator / direction; 

    if(tmin > tmax)
    {
        std::swap(tmin,tmax); 
    }
    std::array<double,2> arr = {tmin,tmax}; 
    return arr; 
}

std::vector<Intersection> Cube::local_intersect(const Ray& r) const
{
    std::vector<Intersection> intersection_list; 
    auto [xtmin,xtmax] = check_axis(r.origin.x,r.direction.x); 
    auto [ytmin,ytmax] = check_axis(r.origin.y,r.direction.y); 
    auto [ztmin,ztmax] = check_axis(r.origin.z,r.direction.z); 

    double tmin = std::max(std::max(xtmin,ytmin),ztmin); 
    double tmax = std::min(std::min(xtmax,ytmax),ztmax); 

    if(tmin > tmax)
        return intersection_list; 

    intersection_list.push_back(Intersection(tmin,this)); 
    intersection_list.push_back(Intersection(tmax,this)); 

    return intersection_list; 
    
}

std::vector<Intersection> Cylinder::local_intersect(const Ray& r) const
{
    std::vector<Intersection> intersection_list; 
    double a = r.direction.x*r.direction.x + r.direction.z * r.direction.z;
    if(a < EPSILON)
    {
        this->intersect_caps(r,intersection_list); 
        return intersection_list; 
    }

    double b = 2 * r.origin.x * r.direction.x + 2*r.origin.z * r.direction.z; 
    double c = r.origin.x * r.origin.x + r.origin.z * r.origin.z - 1; 

    double disc = b*b - 4 * a * c; 

    if(disc < 0.0)
        return intersection_list; 

    double t0 = (-b - sqrt(disc))/(2.0 * a); 
    double t1 = (-b + sqrt(disc))/(2.0 * a); 

    if(t0 > t1)
        std::swap(t0,t1); 

    double y0 = r.origin.y + t0 * r.direction.y; 

    if(this->minimum < y0 && y0 < this->maximum)
        intersection_list.push_back(Intersection(t0,this)); 

    double y1 = r.origin.y + t1 * r.direction.y; 
                  
    if(this->minimum < y1 && y1 < this->maximum)
        intersection_list.push_back(Intersection(t1,this)); 

    this->intersect_caps(r,intersection_list); 

    return intersection_list; 
}

Vector Cylinder::local_normal_at(const Point& object_point) const
{
    double dist = object_point.x * object_point.x + object_point.z * object_point.z; 
    if(dist < 1 && (object_point.y >= this->maximum - EPSILON))
        return Vector(0,1,0); 

    if(dist < 1 && (object_point.y <= this->minimum + EPSILON))
        return Vector(0,-1,0); 

    return Vector(object_point.x,0,object_point.z); 
}

bool Cylinder::check_cap(const Ray& r, double t) const
{
    double x = r.origin.x + t * r.direction.x; 
    double z = r.origin.z + t * r.direction.z; 

    return (x*x + z*z) <= 1; 
}

void Cylinder::intersect_caps(const Ray& r, std::vector<Intersection>& xs) const
{
    //Return if the cylinder is not closed or can't be intersected by the ray
    if(this->type != CYL_TYPE::CLOSED || std::abs(r.direction.y) < EPSILON)
        return; 

    double t = (this->minimum - r.origin.y) / r.direction.y; 
    if(check_cap(r,t))
        xs.push_back(Intersection(t,this)); 

    t = (this->maximum - r.origin.y) / r.direction.y; 
    if(check_cap(r,t))
        xs.push_back(Intersection(t,this)); 
}

Sphere* glass_sphere()
{
    Sphere* s = new Sphere(); 
    s->mat.transparency = 1.0f; 
    s->mat.reflective = 1.0f; 
    s->mat.refractive_index = 1.5; 
    s->mat.specular = 1; 
    s->mat.shininess = 300; 
    return s; 
}

