#include "vector.h"
#include "tools.h"

#include <math.h>
#include <iostream> 

Vector::Vector(double x, double y, double z) 
    : Tuple(x, y, z, 0.0) {}

Vector::Vector(): 
    Tuple(0.0,0.0,0.0,0.0) {}

Vector Vector::operator+(Vector const& obj) const
{
    Vector t; 
    t.x = this->x + obj.x; 
    t.y = this->y + obj.y; 
    t.z = this->z + obj.z; 
    t.w = this->w + obj.w; 
    return t;
}

Vector Vector::operator-(Vector const& obj) const
{
    Vector t; 
    t.x = this->x - obj.x; 
    t.y = this->y - obj.y; 
    t.z = this->z - obj.z; 
    t.w = this->w - obj.w; 
    return t;
}

Vector Vector::operator-() const
{
        return Vector(-x, -y, -z);
};
    
Vector Vector::operator*(double scalar) const
{
    return Vector(scalar * x, scalar * y, scalar * z); 
}  

Vector operator*(double scalar,Vector const& obj)
{
    return Vector(scalar * obj.x, scalar * obj.y, scalar * obj.z); 
}

Vector Vector::operator^(Vector const& obj) const
{
    return Vector(this->y * obj.z - this->z * obj.y, this->z * obj.x - this->x * obj.z, this->x * obj.y - this->y * obj.x); 
}  

double Vector::operator*(Vector const& obj) const
{
    return x * obj.x + y * obj.y + z * obj.z + w * obj.w; 
}  

bool Vector::operator==(Vector const& obj) const
{
    return equal_double(this->x,obj.x) && equal_double(this->y,obj.y) && equal_double(this->z,obj.z) && equal_double(this->w,obj.w); 
} 

double Vector::magnitude() const
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

Vector Vector::reflect_vector(const Vector& normal) const
{
    Vector in = *(this); 
    Vector v = in - (normal * 2 * (in * normal)); 

    return v; 
}

void Vector::printVector() const
{
    std::cout<<this->x<<" "<<this->y<<" "<<this->z<<std::endl; 
}



