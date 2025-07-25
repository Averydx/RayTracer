#include "materials.h"

Material::Material(double ambient, double diffuse, double specular, double shininess,const Color& color)
{
    this->mat_color = color; 
    this->ambient = ambient; 
    this->diffuse = diffuse; 
    this->specular = specular; 
    this->shininess = shininess; 
    this->pattern = nullptr; 
}

Material::Material(double ambient, double diffuse, double specular, double shininess,Pattern* pattern)
{
    this->mat_color = Color(1,1,1); 
    this->ambient = ambient; 
    this->diffuse = diffuse; 
    this->specular = specular; 
    this->shininess = shininess; 
    this->pattern = pattern; 
}
Material::~Material()
{
    delete pattern;
}