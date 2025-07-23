#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "ray.h"
#include "canvas.h"
#include "world.h"

#include <math.h>

class Camera
{
    public: 
        double hsize; 
        double vsize; 

        double field_of_view; 
        Matrix transform; 

        double half_width; 
        double half_height; 

        double pixel_size; 

        //constructors
        Camera(double _hsize, double _vsize, double _fov):hsize(_hsize),vsize(_vsize),field_of_view(_fov),transform(Matrix(4,4)) 
        {
            this->transform.setIdentity();
            
            double half_view = tan(this->field_of_view /2.f); 
            double aspect = this->hsize / this->vsize; 

            if(aspect >= 1)
            {
                this->half_width = half_view; 
                this->half_height = half_view /aspect; 
            }
            else
            {
                this->half_width = half_view * aspect; 
                this->half_height = half_view; 
            }

            this->pixel_size = (this->half_width * 2)/this->hsize; 
        }

        //methods 
        Ray ray_for_pixel(double px, double py) const;      
}; 

Canvas render(const Camera& c, const World& w); 



#endif