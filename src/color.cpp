#include <color.h>
#include "tools.h"

Color::Color(double r, double g, double b) 
    : Tuple(r, g, b, 0.0) {}

bool Color::operator==(Color const& obj)
{
    return this->x == obj.x && this->y == obj.y && this->z == obj.z && this->w == obj.w; 
} 

Color Color::operator-(Color const& obj)
{
    return Color(this->x - obj.x,this->y - obj.y, this->z - obj.z); 
}

Color Color::operator+(Color const& obj)
{
    return Color(this->x + obj.x,this->y + obj.y, this->z + obj.z); 
}


Color Color::operator*(Color const& obj)
{
    return Color(this->x * obj.x,this->y * obj.y, this->z * obj.z); 
}

Color Color::operator*(double scalar)
{
    return Color(scalar * this->x,this->y * scalar, this->z * scalar); 
}

