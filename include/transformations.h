#ifndef TRANS_H
#define TRANS_H

#include "matrix.h"
#include "point.h"
#include "vector.h"

// This file defines transformation functions for 3D graphics
// It includes functions for translation, scaling, rotation, and skewing
Matrix translation(double x, double y, double z); 
Matrix scaling(double x, double y, double z); 
Matrix rotation_x(double rad); 
Matrix rotation_y(double rad); 
Matrix rotation_z(double rad); 
Matrix skew(double x_y,double x_z,double y_x,double y_z,double z_x,double z_y); 

//view_transform function creates a view transformation matrix
// It takes a point representing the camera's position, a point to look at, and a vector representing the up direction
Matrix view_transform(const Point& from, const Point& to, const Vector& up); 

#endif