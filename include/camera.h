#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "ray.h"
#include "canvas.h"
#include "world.h"
#include <math.h>

// This file defines the Camera class for rendering scenes in a ray tracer
// It includes methods for creating rays for each pixel and rendering the scene
// The Camera class is responsible for defining the view of the scene and generating rays based on pixel coordinates
class Camera
{
    public: 
        //fields
        int hsize; 
        int vsize; 

        double field_of_view; 
        Matrix transform; 

        double half_width; 
        double half_height; 

        double pixel_size; 

        //constructors
        Camera(int _hsize, int _vsize, double _fov):hsize(_hsize),vsize(_vsize),field_of_view(_fov),transform(Matrix(4,4)) 
        {
            this->transform.setIdentity();
            
            double half_view = tan(this->field_of_view /2.f); 
            double aspect = (double)this->hsize / (double)this->vsize; 

            // Calculate half width and height based on aspect ratio
            // This ensures that the camera's view is correctly proportioned
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
        // Generates a ray for a specific pixel in the camera's view
        Ray ray_for_pixel(double px, double py) const;      
}; 

// Function to render the scene from the camera's perspective
Canvas render(const Camera& c, World& w); 



#endif