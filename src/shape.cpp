#include "shape.h"
#include "materials.h"
#include "ray.h"

Matrix Shape::getTransform() const
{
    return this->transform; 
}

void Shape::setTransform(const Matrix& m)
{
    this->transform = m; 
}

void Shape::setMaterial(const Material& m)
{
    this->mat = m; 
}

Material Shape::getMaterial() const
{
    return this->mat; 
}

std::vector<Intersection> Shape::intersect(const Ray& r) const
{
    Ray local_ray = r.ray_transform(this->transform.inverse()); 
    return this->local_intersect(local_ray); 
}


Vector Shape::normal_at(const Point& world_point) const
{
    Point local_point = (this->transform.inverse()) * world_point; 
    Vector local_normal = this->local_normal_at(local_point); 
    Matrix trans_inv = this->transform.inverse(); 
    trans_inv.transpose(); 
    Vector world_normal =  trans_inv * local_normal; 
    world_normal.w = 0; 

    return world_normal.normalize(); 
}
