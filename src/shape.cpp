#include "shape.h"
#include "materials.h"
#include "ray.h"

#include <algorithm> 

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

int scan_container(const std::vector<const Shape*>& container,const Shape* desired)
{
    for(int i = 0; i < container.size();i++)
    {
        if(container[i] == desired)
            return i; 
    }
    return -1; 
}

AABB AABB::transform(const Matrix& mat)
{
    std::vector<Point> vertices = {
        Point(this->minimum.x,this->minimum.y,this->minimum.z),
        Point(this->maximum.x,this->minimum.y,this->minimum.z),
        Point(this->minimum.x,this->maximum.y,this->minimum.z),
        Point(this->minimum.x,this->minimum.y,this->maximum.z),
        Point(this->maximum.x,this->maximum.y,this->minimum.z),
        Point(this->minimum.x,this->maximum.y,this->maximum.z),
        Point(this->maximum.x,this->minimum.y,this->maximum.z),
        Point(this->maximum.x,this->maximum.y,this->maximum.z)
    }; 

    for(Point& v: vertices)
    {
        v = mat * v; 
    }

    Point minPoint = vertices[0]; 
    Point maxPoint = vertices[1]; 

    for(Point& v: vertices)
    {
        minPoint.x = std::min(v.x,minPoint.x); 
        minPoint.y = std::min(v.y,minPoint.y); 
        minPoint.z = std::min(v.z,minPoint.z); 

        maxPoint.x = std::max(v.x,maxPoint.x); 
        maxPoint.y = std::max(v.y,maxPoint.y); 
        maxPoint.z = std::max(v.z,maxPoint.z); 
    }

    return AABB(minPoint,maxPoint); 
}

std::array<double,2> AABB::check_axis(double origin, double direction,AXIS ax) const
{
    double tmin,tmax,tmin_numerator,tmax_numerator; 

    switch (ax)
    {
    case AXIS::X_AXIS:
        tmin_numerator = (minimum.x- origin); 
        tmax_numerator = (maximum.x - origin); 

        tmin = tmin_numerator / direction; 
        tmax = tmax_numerator / direction; 
        break;
    
    case AXIS::Y_AXIS:
        tmin_numerator = (minimum.y - origin); 
        tmax_numerator = (maximum.y - origin); 

        tmin = tmin_numerator / direction; 
        tmax = tmax_numerator / direction; 
        break;

    case AXIS::Z_AXIS:
        tmin_numerator = (minimum.z- origin); 
        tmax_numerator = (maximum.z - origin); 

        tmin = tmin_numerator / direction; 
        tmax = tmax_numerator / direction; 
        break;

    }

    if(tmin > tmax)
    {
        std::swap(tmin,tmax); 
    }
    std::array<double,2> arr = {tmin,tmax}; 
    return arr;
}

bool AABB::check_intersect(const Ray& r) const
{
    auto [xtmin,xtmax] = check_axis(r.origin.x,r.direction.x,AXIS::X_AXIS); 
    auto [ytmin,ytmax] = check_axis(r.origin.y,r.direction.y,AXIS::Y_AXIS); 
    auto [ztmin,ztmax] = check_axis(r.origin.z,r.direction.z,AXIS::Z_AXIS); 

    double tmin = std::max(std::max(xtmin,ytmin),ztmin); 
    double tmax = std::min(std::min(xtmax,ytmax),ztmax); 

    if(tmin > tmax)
        return false;

    return true; 
}
