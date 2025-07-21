#ifndef SPHERE_H
#define SPHERE_H

#include "point.h"
#include "intersection.h"
#include "shape.h"
#include "matrix.h"
#include "ray.h"
#include "vector.h"
#include "point.h"
#include "materials.h"


class Sphere : public Shape
{
    public:

        //Constructors
        Sphere():transform(Matrix(4,4)),mat(Material()) {this->transform.setIdentity();}
        std::vector<Intersection*> intersect(const Ray& r) const override; 

        //getters-setters
        void setTransform(const Matrix& m); 
        Matrix getTransform() const; 

        void setMaterial(const Material& m); 
        Material getMaterial() const; 

        //methods
        Vector normal_at(const Point& world_point) const; 

    private: 
        //fields
        Matrix transform;  
        Material mat;
}; 

#endif