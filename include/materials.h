#ifndef MAT_H
#define MAT_H

#include "color.h"
#include "pattern.h"

class Material
{
    public: 
        Color mat_color; 
        Pattern* pattern; 
        double ambient; 
        double diffuse; 
        double specular; 
        double shininess; 
        double reflective = 0.f; 

        //constructor
        Material():ambient(0.1),diffuse(0.9),specular(0.9),shininess(200.0),mat_color(Color(1.0,1.0,1.0)),pattern(nullptr) {}; 
        Material(double ambient, double diffuse, double specular, double shininess,const Color& color); 
        Material(double ambient, double diffuse, double specular, double shininess,Pattern* pattern); 

        //destructor
        ~Material(); 
}; 

#endif