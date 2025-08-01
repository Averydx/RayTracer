#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "intersection.h"
#include "materials.h"
#include "matrix.h"
#include "tools.h"

#include <vector> 
#include <limits>
#include <array>

class AABB
{
    public: 
        Point minimum = Point(-EPSILON,-EPSILON,-EPSILON); 
        Point maximum = Point(EPSILON,EPSILON,EPSILON);  

        AABB(Point min, Point max):minimum(min),maximum(max){}
        AABB(){}
        ~AABB() = default; 

        AABB transform(const Matrix& mat); 
        bool check_intersect(const Ray& r) const; 

        //helper functions
        std::array<double,2> AABB::check_axis(double origin, double direction,AXIS ax) const;
}; 

class Shape
{
    public: 
        Shape():transform(Matrix(4,4)),mat(Material()) {this->transform.setIdentity();}
        virtual ~Shape() = default;

        std::vector<Intersection> intersect(const Ray& r) const;
        Vector normal_at(const Point& world_point) const; 

        virtual std::vector<Intersection> local_intersect(const Ray& r) const = 0; 
        virtual Vector local_normal_at(const Point& object_point) const = 0; 
        virtual AABB bounds() const = 0; 

        void setTransform(const Matrix& m); 
        Matrix getTransform() const; 

        void setMaterial(const Material& m); 
        Material getMaterial() const; 

        Matrix transform;  
        Material mat;
}; 

int scan_container(const std::vector<const Shape*>& container,const Shape* desired); 


#endif