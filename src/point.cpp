#include "point.h"
#include "tools.h"

#include <iostream>

Point::Point(double x, double y, double z) 
    : Tuple(x, y, z, 1.0) {}

Point::Point()
    : Tuple(0, 0, 0, 1.0) {}

bool Point::operator==(Point const& obj)
{
    return equal_double(this->x,obj.x) && equal_double(this->y,obj.y) && equal_double(this->z,obj.z) && equal_double(this->w,obj.w); 
} 

Vector Point::operator-(Point const& obj)
{
    return Vector(this->x - obj.x,this->y - obj.y, this->z - obj.z); 
}

Point Point::operator+(Vector const& obj)
{
    return Point(this->x + obj.x,this->y + obj.y, this->z + obj.z);
}

void Point::printPoint()
{
    std::cout<< this->x << " " << this->y << " " << this->z << std::endl; 
}
