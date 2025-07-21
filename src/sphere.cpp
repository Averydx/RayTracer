#include "sphere.h"

std::vector<Intersection*> Sphere::intersect(const Ray& r) const
{

    std::vector<Intersection*> intersections; 

    Matrix s_trans_inv = this->transform.inverse(); 
    Ray t_ray = r.ray_transform(s_trans_inv); 

    Vector sphere_to_ray = t_ray.origin - Point(0.,0.,0.); 

    double a = t_ray.direction * t_ray.direction; 
    double b = 2 * (t_ray.direction * sphere_to_ray); 
    double c = (sphere_to_ray * sphere_to_ray) - 1; 

    double discriminant = b*b - 4 * a * c; 

    if(discriminant < 0)
        return intersections; 

    intersections.push_back(new Intersection((-b - sqrt(discriminant))/(2*a),this)); 
    intersections.push_back(new Intersection((-b + sqrt(discriminant))/(2*a),this));
    
    return intersections; 

}

Matrix Sphere::getTransform() const
{
    return this->transform; 
}

void Sphere::setTransform(const Matrix& m)
{
    this->transform = m; 
}

void Sphere::setMaterial(const Material& m)
{
    this->mat = m; 
}

Material Sphere::getMaterial() const
{
    return this->mat; 
}

Vector Sphere::normal_at(const Point& world_point) const
{
    Point object_point = this->getTransform().inverse() * world_point; 
    Vector object_normal = object_point - Point(0.,0.,0.); 
    
    Matrix m = this->getTransform().inverse(); 
    m.transpose();  
    Vector world_normal =  m * object_normal; 
    world_normal.w = 0; 
    world_normal.normalize(); 
    
    return world_normal; 
}