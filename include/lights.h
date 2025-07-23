#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"
#include "color.h"
#include "vector.h"
#include "materials.h"
#include "lights.h"

class pointLight
{
    public: 
        Color intensity; 
        Point position; 

        //constructor
        pointLight(Color light_inten, Point light_pos); 
        pointLight(); 
}; 

Color lighting(const Material& mat, const pointLight& light, const Point& point, const Vector& eye_vec, const Vector& normal_vec); 

#endif