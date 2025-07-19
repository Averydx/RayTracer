#ifndef TRANS_H
#define TRANS_H

#include "matrix.h"

Matrix translation(double x, double y, double z); 
Matrix scaling(double x, double y, double z); 
Matrix rotation_x(double rad); 
Matrix rotation_y(double rad); 
Matrix rotation_z(double rad); 
Matrix skew(double x_y,double x_z,double y_x,double y_z,double z_x,double z_y); 

#endif