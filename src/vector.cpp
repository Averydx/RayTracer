#include <vector.h>
#include <cmath>
#include "tools.h"

Vector::Vector(double x, double y, double z) 
    : Tuple(x, y, z, 0.0) {}

Vector::Vector(): 
    Tuple(0.0,0.0,0.0,0.0) {}

Vector Vector::operator+(Vector const& obj)
{
    Vector t; 
    t.x = this->x + obj.x; 
    t.y = this->y + obj.y; 
    t.z = this->z + obj.z; 
    t.w = this->w + obj.w; 
    return t;
}

Vector Vector::operator-(Vector const& obj)
{
    Vector t; 
    t.x = this->x - obj.x; 
    t.y = this->y - obj.y; 
    t.z = this->z - obj.z; 
    t.w = this->w - obj.w; 
    return t;
}

Vector Vector::operator-(){
        return Vector(-x, -y, -z);
    };
    
Vector Vector::operator*(double scalar)
{
    return Vector(scalar * x, scalar * y, scalar * z); 
}  

Vector operator*(double scalar,Vector const& obj)
{
    return Vector(scalar * obj.x, scalar * obj.y, scalar * obj.z); 
}

Vector Vector::operator^(Vector const& obj)
{
    return Vector(this->y * obj.z - this->z * obj.y, this->z * obj.x - this->x * obj.z, this->x * obj.y - this->y * obj.x); 
}  

double Vector::operator*(Vector const& obj)
{
    return x * obj.x + y * obj.y + z * obj.z + w * obj.w; 
}  

bool Vector::operator==(Vector const& obj)
{
    return equal_double(this->x,obj.x) && equal_double(this->y,obj.y) && equal_double(this->z,obj.z) && equal_double(this->w,obj.w); 
} 

double Vector::magnitude()
{
    return sqrt(x*x + y*y + z*z + w*w); 
}

void Vector::normalize()
{
    double mag = this->magnitude(); 
    this->x /= mag; 
    this->y /= mag; 
    this->z /= mag; 
    this->w /= mag; 
}



