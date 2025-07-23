#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include <vector> 
#include "intersection.h"
#include "materials.h"
#include "matrix.h"

class Shape
{
    public: 
        Shape():transform(Matrix(4,4)),mat(Material()) {this->transform.setIdentity();}
        virtual ~Shape() = default;
        virtual std::vector<Intersection> intersect(const Ray& r) const = 0;
        virtual Vector normal_at(const Point& world_point) const = 0; 

        void setTransform(const Matrix& m); 
        Matrix getTransform() const; 

        void setMaterial(const Material& m); 
        Material getMaterial() const; 

    // private:
        Matrix transform;  
        Material mat;
}; 

#endif