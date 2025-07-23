#include "sphere.h"

std::vector<Intersection> Sphere::intersect(const Ray& r) const
{

    std::vector<Intersection> intersection_list; 

    Matrix s_trans_inv = this->getTransform().inverse(); 
    Ray t_ray = r.ray_transform(s_trans_inv); 

    Vector sphere_to_ray = t_ray.origin - Point(0.,0.,0.); 

    double a = t_ray.direction * t_ray.direction; 
    double b = 2 * (t_ray.direction * sphere_to_ray); 
    double c = (sphere_to_ray * sphere_to_ray) - 1; 

    double discriminant = b*b - 4 * a * c; 

    if(discriminant < 0)
        return intersection_list; 

    intersection_list.push_back(Intersection((-b - sqrt(discriminant))/(2*a),this)); 
    intersection_list.push_back(Intersection((-b + sqrt(discriminant))/(2*a),this));
    
    return intersection_list; 

}

Vector Sphere::normal_at(const Point& world_point) const
{
    Point object_point = this->getTransform().inverse() * world_point; 
    Vector object_normal = object_point - Point(0.,0.,0.); 
    
    Matrix m = this->getTransform().inverse(); 
    m.transpose();  
    Vector world_normal =  m * object_normal; 
    world_normal.w = 0; 
    return world_normal.normalize(); 

}