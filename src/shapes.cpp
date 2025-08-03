#include "shapes.h"
#include "tools.h"
#include "transformations.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm> 
#include <iostream> 


AABB Sphere::bounds() const
{
    return AABB(Point(-1,-1,-1),Point(1,1,1)); 
}

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

Vector Sphere::local_normal_at(const Point& object_point,const Intersection& hit) const 
{
    
    Vector local_normal = object_point - Point(0.,0.,0.); 
    return local_normal; 

}

AABB Plane::bounds() const
{
    return AABB(Point(this->x_minimum,-EPSILON,this->z_minimum),Point(this->x_maximum,EPSILON,this->z_maximum)); 
}

Vector Plane::local_normal_at(const Point& object_point,const Intersection& hit) const 
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

AABB Cube::bounds() const
{
    return AABB(Point(-1,-1,-1),Point(1,1,1)); 
}

Vector Cube::local_normal_at(const Point& object_point,const Intersection& hit) const 
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

Vector Cylinder::local_normal_at(const Point& object_point,const Intersection& hit) const 
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

AABB Cylinder::bounds() const
{
    return AABB(Point(-1,this->minimum,-1),Point(1,this->maximum,1)); 
}

Group::Group():Shape()
{
    this->isGroup = true; 
    this->refresh_bvh();  
}

std::vector<Intersection> Group::local_intersect(const Ray& r) const
{
    std::vector<Intersection> xs = bvh_intersect(this->bvh,r); 
    // std::vector<Intersection> xs; 
    // for(Shape* s: this->children)
    // {
    //     std::vector<Intersection> temp = s->intersect(r); 
    //     xs.insert(xs.end(),temp.begin(),temp.end()); 
    // }

    std::sort(xs.begin(),xs.end(),comp_intersection); 

    return xs; 
} 

//methods
Vector Group::local_normal_at(const Point& object_point,const Intersection& hit) const 
{
    return Vector(0,0,0); 
}

AABB Group::bounds() const
{
    AABB bbox; 
    for(Shape* child:this->children)
    {
       AABB child_box = child->bounds();
       child_box = child_box.transform(child->transform);
       bbox = box_union(bbox,child_box); 
    }

    return bbox; 
}

void Group::add_child(Shape* s)
{
    s->parent = this; 
    this->children.push_back(s); 
}

Group::~Group()
{
    if(!this->children.empty())
    {
        for(Shape* s: this->children)
        {
            delete s; 
        }
    }
}

void Group::percolate_material()
{
    for(Shape* child: this->children)
    {
        child->mat = this->mat; 
        if(child->isGroup)
        {
            Group* g = static_cast<Group*>(child); 
            g->percolate_material(); 
        }
    }
}

void Group::refresh_bvh()
{
    delete_bvh(this->bvh); 
    this->bvh = build_bvh(this->children,2); 

    for(Shape* s: this->children)
    {
        if(s->isGroup)
        {
            Group* g = static_cast<Group*>(s); 
            g->refresh_bvh(); 
        }
    }
}

std::vector<Intersection> Triangle::local_intersect(const Ray& r) const 
{
    std::vector<Intersection> xs = std::vector<Intersection>(); 
    Vector dir_cross_e2 = r.direction ^ this->e2; 
    double det = this->e1 * dir_cross_e2; 

    //Check if the ray is parallel to the triangle
    if(abs(det)<EPSILON)
        return xs; 

    //Checks if the ray misses the p1-p3 edge
    double f = 1.0/det; 
    Vector p1_to_origin = r.origin - this->p1; 
    double u = f * (p1_to_origin * dir_cross_e2); 

    if(u < 0 || u > 1)
        return xs;
      
    Vector origin_cross_e1 = p1_to_origin ^ this->e1; 
    double v = f * (r.direction * origin_cross_e1); 

    if(v < 0 || ((u + v) > 1))
        return xs; 

    double t = f * (this->e2 * origin_cross_e1); 
    xs.push_back(Intersection(t,this,u,v)); 

    return xs; 
    
}
Vector Triangle::local_normal_at(const Point& object_point,const Intersection& hit) const 
{
    return this->normal; 
}
AABB Triangle::bounds() const
{
    double x_min = std::min(std::min(p1.x,p2.x),p3.x); 
    double y_min = std::min(std::min(p1.y,p2.y),p3.y);
    double z_min = std::min(std::min(p1.z,p2.z),p3.z);

    double x_max = std::max(std::max(p1.x,p2.x),p3.x); 
    double y_max = std::max(std::max(p1.y,p2.y),p3.y);
    double z_max = std::max(std::max(p1.z,p2.z),p3.z);
    

    return AABB(Point(x_min,y_min,z_min),Point(x_max,y_max,z_max)); 
}

std::vector<Intersection> SmoothTriangle::local_intersect(const Ray& r) const
{
    std::vector<Intersection> xs = std::vector<Intersection>(); 
    Vector dir_cross_e2 = r.direction ^ this->e2; 
    double det = this->e1 * dir_cross_e2; 

    //Check if the ray is parallel to the triangle
    if(abs(det)<EPSILON)
        return xs; 

    //Checks if the ray misses the p1-p3 edge
    double f = 1.0/det; 
    Vector p1_to_origin = r.origin - this->p1; 
    double u = f * (p1_to_origin * dir_cross_e2); 

    if(u < 0 || u > 1)
        return xs;
      
    Vector origin_cross_e1 = p1_to_origin ^ this->e1; 
    double v = f * (r.direction * origin_cross_e1); 

    if(v < 0 || ((u + v) > 1))
        return xs; 

    double t = f * (this->e2 * origin_cross_e1); 
    xs.push_back(Intersection(t,this,u,v)); 

    return xs;  
}

Vector SmoothTriangle::local_normal_at(const Point& object_point,const Intersection& hit) const 
{
    return (this->n2 * hit.u + this->n3 * hit.v + this->n1 * (1-hit.u - hit.v)); 
} 
AABB SmoothTriangle::bounds() const
{
    double x_min = std::min(std::min(p1.x,p2.x),p3.x); 
    double y_min = std::min(std::min(p1.y,p2.y),p3.y);
    double z_min = std::min(std::min(p1.z,p2.z),p3.z);

    double x_max = std::max(std::max(p1.x,p2.x),p3.x); 
    double y_max = std::max(std::max(p1.y,p2.y),p3.y);
    double z_max = std::max(std::max(p1.z,p2.z),p3.z);
    

    return AABB(Point(x_min,y_min,z_min),Point(x_max,y_max,z_max));  
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

Sphere* hexagon_corner()
{
    Sphere* corner = new Sphere(); 
    corner->transform = translation(0,0,-1) * scaling(0.25,0.25,0.25); 

    return corner; 
}

Cylinder* hexagon_edge()
{
    Cylinder* edge = new Cylinder(0,1);
    edge->transform = translation(0,0,-1) * rotation_y(-M_PI/6.0) * rotation_z(-M_PI/2.0) * scaling(0.25,1,0.25); 
    
    return edge; 
}

Group* hexagon_side()
{
    Group* side = new Group(); 
    side->add_child(hexagon_corner()); 
    side->add_child(hexagon_edge()); 

    return side; 
}

Group* hexagon()
{
    Group* hex = new Group(); 
    for(int n = 0; n < 6; n++)
    {
        Group* side = hexagon_side(); 
        side->transform = rotation_y(n * M_PI/3.0); 
        hex->add_child(side); 
    }

    return hex; 
}







