#include "point.h"
#include "vector.h"
#include "canvas.h"
#include "matrix.h"
#include "transformations.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

int main(int argc, char *argv[])
{
    int width = 50; 
    int height = 50; 
    Canvas c = Canvas(width,height); 

    Point p(0,0,1); 

    double clock_radius = 20; 

    for(int i = 0; i < 12; i++)
    {
        Matrix A = rotation_y(i * M_PI/6); 
        Point temp = A * p; 

        c.SetPixel(25 + (temp.x)*clock_radius,25 + (temp.z * clock_radius),Color(255,0,0));
    }

    c.CanvasToPPM("demo2.ppm"); 
}