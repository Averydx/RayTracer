#include "transformations.h"
#include <math.h>

Matrix translation(double x, double y, double z)
{
    Matrix A(4,4); 
    A.setIdentity(); 
    A.setElement(0,3,x);  
    A.setElement(1,3,y);
    A.setElement(2,3,z);
    A.setElement(3,3,1);

    return A; 
}

Matrix scaling(double x, double y, double z)
{
    Matrix A(4,4); 
    A.setIdentity(); 
    A.setElement(0,0,x); 
    A.setElement(1,1,y); 
    A.setElement(2,2,z); 

    return A; 
}

Matrix rotation_x(double rad)
{
    Matrix A(4,4); 
    A.setIdentity(); 
    A.setElement(1,1,cos(rad)); 
    A.setElement(2,2,cos(rad)); 
    A.setElement(2,1,sin(rad)); 
    A.setElement(1,2,-sin(rad)); 

    return A; 
}

Matrix rotation_y(double rad)
{
    Matrix A(4,4); 
    A.setIdentity(); 
    A.setElement(0,0,cos(rad)); 
    A.setElement(2,2,cos(rad)); 
    A.setElement(0,2,sin(rad)); 
    A.setElement(2,0,-sin(rad)); 

    return A; 
}

Matrix rotation_z(double rad)
{
    Matrix A(4,4); 
    A.setIdentity(); 
    A.setElement(0,0,cos(rad)); 
    A.setElement(1,1,cos(rad)); 
    A.setElement(0,1,-sin(rad)); 
    A.setElement(1,0,sin(rad)); 

    return A; 
}

Matrix skew(double x_y,double x_z,double y_x,double y_z,double z_x,double z_y)
{
    Matrix A(4,4); 
    A.setIdentity();
    
    A.setElement(0,1,x_y); 
    A.setElement(0,2,x_z); 
    A.setElement(1,0,y_x); 
    A.setElement(2,0,z_x); 
    A.setElement(2,1,z_y); 
    A.setElement(1,2,y_z); 

    return A; 
}

Matrix view_transform(const Point& from, const Point& to, const Vector& up)
{
    Vector forward = to - from; 
    forward = forward.normalize(); 
    Vector upn = up.normalize(); 
    Vector left = forward ^ upn; 
    Vector true_up = left ^ forward; 

    std::vector<double> data = {
        left.x,left.y,left.z,0,
        true_up.x, true_up.y, true_up.z,0,
        -forward.x, -forward.y, -forward.z, 0, 
        0, 0, 0, 1
    }; 

    Matrix orientation(4,4,data); 

    return orientation * translation(-from.x,-from.y,-from.z); 
}
