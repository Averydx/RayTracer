#include <point.h>
#include "tools.h"

Point::Point(double x, double y, double z) 
    : Tuple(x, y, z, 1.0) {}

bool Point::operator==(Point const& obj)
{
    return this->x == obj.x && this->y == obj.y && this->z == obj.z && this->w == obj.w; 
} 

Vector Point::operator-(Point const& obj)
{
    return Vector(this->x - obj.x,this->y - obj.y, this->z - obj.z); 
}

