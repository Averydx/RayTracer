#include "materials.h"

Material::Material(double ambient, double diffuse, double specular, double shininess,const Color& color)
{
    this->mat_color = color; 
    this->ambient = ambient; 
    this->diffuse = diffuse; 
    this->specular = specular; 
    this->shininess = shininess; 
}