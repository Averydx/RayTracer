#include "ray.h"
#include "canvas.h"
#include "sphere.h"
#include "intersection.h"
#include "vector.h"
#include "point.h"
#include "transformations.h"
#include "materials.h"
#include "lights.h"

#include <vector> 


int main(int argc, char *argv[])
{
    Point ray_origin(0.f,0.f,-5.f);

    double wall_z = 10.f; 
    double wall_size = 30.f; 

    int width = 1000; 
    int height = 1000; 
    Canvas c = Canvas(width,height); 

    double pixel_size = wall_size / width; 
    double half = wall_size/2; 

    Sphere s; 
    s.setMaterial(Material(0.1,0.9,0.9,200.0,Color(1,0.2,1))); 

    pointLight light(Color(1,1,1),Point(-10,10,-10)); 

    for(int y = 0; y < height; y++)
    {
        double world_y = half - pixel_size * y; 

        for(int x = 0; x < width; x++)
        {
            double world_x = -half + pixel_size * x; 
            Point position(world_x,world_y,wall_z); 

            Vector dir = position - ray_origin;
            dir.normalize();
            Ray r(ray_origin,dir);

            std::vector<Intersection*> xs = s.intersect(r); 

            Intersection* hit = find_hit(xs); 
            if(hit != nullptr)
            {
                Point p = r.position(hit->t); 
                const Sphere* sphere = dynamic_cast<const Sphere*>(hit->s); 
                Vector normal = sphere->normal_at(p); 

                Vector eye = -r.direction; 

                Color color = lighting(sphere->getMaterial(),light,p,eye,normal); 

                c.SetPixel(x,y,color);
            }
                
        }
    }

    c.CanvasToPPM("demo3.ppm"); 
}