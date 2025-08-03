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
        //fields
        Point minimum = Point(std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()); // Minimum corner of the AABB
        Point maximum = Point(-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity());  // Maximum corner of the AABB

        AABB(Point min, Point max):minimum(min),maximum(max){} // Constructor to initialize the AABB with minimum and maximum points
        AABB(){} // Default constructor initializes to a small AABB around the origin
        ~AABB() = default; // Default destructor

        AABB transform(const Matrix& mat); // Transforms the AABB using a transformation matrix
        bool check_intersect(const Ray& r) const; // Checks if a ray intersects the AABB

        //helper functions
        std::array<double,2> AABB::check_axis(double origin, double direction,AXIS ax) const; // Checks intersection along a specific axis
}; 

/*
 * Base class for all shapes
 * Provides common functionality for intersection and normal calculation
 * Derived classes must implement local_intersect, local_normal_at, and bounds methods
 */
class Shape
{
    public: 
        Shape():transform(Matrix(4,4)),mat(Material()) {this->transform.setIdentity();} // Default constructor initializes the shape with an identity transformation and default material
        virtual ~Shape() = default; // Default destructor

        std::vector<Intersection> intersect(const Ray& r) const; // Intersects a ray with the shape, returning a list of intersections
        Vector normal_at(const Point& world_point,const Intersection& hit) const; // Calculates the normal vector at a given point in world coordinates

        virtual std::vector<Intersection> local_intersect(const Ray& r) const = 0; // Pure virtual method for local intersection, must be implemented by derived classes
        virtual Vector local_normal_at(const Point& object_point,const Intersection& hit) const = 0; // Pure virtual method for local normal calculation, must be implemented by derived classes
        virtual AABB bounds() const = 0; // Pure virtual method for bounding box calculation, must be implemented by derived classes

        void setTransform(const Matrix& m); // Sets the transformation matrix for the shape
        Matrix getTransform() const; // Gets the transformation matrix of the shape

        void setMaterial(const Material& m); // Sets the material for the shape
        Material getMaterial() const; // Gets the material of the shape

        Matrix transform;  // Transformation matrix for the shape
        Material mat; // Material properties of the shape, can be used for shading and rendering
        Shape* parent = nullptr; // Pointer to the parent shape, if any, used for hierarchical transformations
        bool isGroup = false; 
}; 

int scan_container(const std::vector<const Shape*>& container,const Shape* desired); // Scans a container of shapes to find the index of a desired shape, returns -1 if not found

Point world_to_object(const Shape* shape, Point point); 
Vector normal_to_world(const Shape* shape,Vector normal);

#endif