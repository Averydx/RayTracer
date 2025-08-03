#include "camera.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "ray.h"
#include "bvh.h"

#include <iomanip>
#include <iostream>
#include <omp.h>

Ray Camera::ray_for_pixel(double px, double py) const
{
    //The offset from the edge of the canvas to the pixel's center
    double xoffset = (px + 0.5) * this->pixel_size; 
    double yoffset = (py + 0.5) * this->pixel_size;

    //The untransformed corridnates of the pixel in world space
    double world_x = this->half_width - xoffset; 
    double world_y = this->half_height - yoffset; 

    Matrix inv_trans = this->transform.inverse(); 
    Point pixel = inv_trans * Point(world_x,world_y,-1); 
    Point origin = inv_trans * Point(0,0,0); 
    Vector direction = (pixel - origin); 
    direction = direction.normalize(); 

    return Ray(origin, direction); 
} 


Canvas render(const Camera& c, World& w)
{
    Canvas image(c.hsize,c.vsize); 

    int total_pixel_count = c.hsize * c.vsize; 

    #pragma omp parallel for default(none)
        for(int i = 0; i < total_pixel_count;i++)
        {
            int x = i % c.hsize; 
            int y = i / c.hsize; 

            Ray r = c.ray_for_pixel(x,y); 
            Color color = w.color_at(r); 

            image.SetPixel(x,y,color); 

        }

    return image; 
}