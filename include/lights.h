#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"
#include "color.h"
#include "vector.h"
#include "materials.h"
#include "lights.h"

// This file defines the pointLight class and lighting function for ray tracing
// The pointLight class represents a light source in the scene
class pointLight
{
    public: 
        Color intensity; 
        Point position; 

        //constructor
        pointLight(Color light_inten, Point light_pos); 
        pointLight(); 
}; 

// Function to calculate the lighting at a point in the scene
// It takes into account the material properties, light position, eye vector, and normal vector
Color lighting(const Material& mat, const Shape* shape, const pointLight& light, const Point& point, const Vector& eye_vec, const Vector& normal_vec,bool in_shadow = false); 

#endif