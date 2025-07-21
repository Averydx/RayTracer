#ifndef MAT_H
#define MAT_H

#include "color.h"

class Material
{
    public: 
        Color mat_color; 
        double ambient; 
        double diffuse; 
        double specular; 
        double shininess; 

        //constructor
        Material():ambient(0.1),diffuse(0.9),specular(0.9),shininess(200.0),mat_color(Color(1.0,1.0,1.0)) {}; 
        Material(double ambient, double diffuse, double specular, double shininess,const Color& color); 
}; 

#endif