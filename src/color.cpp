#include <color.h>
#include "tools.h"

Color::Color(double r, double g, double b) 
    : Tuple(r, g, b, 0.0) {}

Color::Color()
    :Tuple(0.0,0.0,0.0,0.0) {}

bool Color::operator==(Color const& obj) const
{
    return equal_double(this->x,obj.x) && equal_double(this->y,obj.y) && equal_double(this->z,obj.z) && equal_double(this->w,obj.w); 
} 

Color Color::operator-(Color const& obj) const
{
    return Color(this->x - obj.x,this->y - obj.y, this->z - obj.z); 
}

Color Color::operator+(Color const& obj) const
{
    return Color(this->x + obj.x,this->y + obj.y, this->z + obj.z); 
}


Color Color::operator*(Color const& obj) const
{
    return Color(this->x * obj.x,this->y * obj.y, this->z * obj.z); 
}

Color Color::operator*(double scalar) const
{
    return Color(scalar * this->x,this->y * scalar, this->z * scalar); 
}

void Color::clamp(int min, int max)
{
    if(this->x > max)
    {
        this->x = max; 
    }
    if(this->x < min)
    {
        this->x = min; 
    }

    if(this->y > max)
    {
        this->y = max; 
    }
    if(this->y < min)
    {
        this->y= min; 
    }

    if(this->z > max)
    {
        this->z = max; 
    }

    if(this->z < min)
    {
        this->z = min; 
    }

    this->x = round(x); 
    this->y = round(y); 
    this->z = round(z); 
}

