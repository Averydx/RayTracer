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
