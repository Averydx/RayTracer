#include "ray.h"
#include "canvas.h"
#include "shapes.h"
#include "intersection.h"
#include "vector.h"
#include "point.h"
#include "transformations.h"
#include "materials.h"
#include "lights.h"
#include "camera.h"
#include "world.h"

#include <vector> 
#define _USE_MATH_DEFINES
#include <math.h>


int main(int argc, char *argv[])
{
    Plane* floor = new Plane(); 
    Plane* wall = new Plane(); 

    wall->setTransform(translation(0,0,5)*rotation_x(M_PI/2.f)); 

    // Sphere* floor = new Sphere(); 
    // floor->transform = scaling(10,0.01,10); 
    // floor->mat.mat_color = Color(1,0.9,0.9); 
    // floor->mat.specular = 0; 

    // Sphere* left_wall = new Sphere(); 
    // left_wall->transform = (translation(0,0,5) * rotation_y(-M_PI/4.f) * rotation_x(M_PI/2.f) * scaling(10,0.01,10)); 
    // left_wall->mat = floor->mat; 

    // Sphere* right_wall = new Sphere(); 
    // right_wall->transform = (translation(0,0,5) * rotation_y(M_PI/4.f) * rotation_x(M_PI/2.f) * scaling(10,0.01,10)); 
    // right_wall->mat = floor->mat; 

    Sphere* middle = new Sphere(); 
    middle->transform = translation(-0.5,1,0.5); 
    middle->mat = Material(); 
    middle->mat.mat_color = Color(0.1,1,0.5); 
    middle->mat.diffuse = 0.7; 
    middle->mat.specular = 0.3; 

    Sphere* right = new Sphere(); 
    right->transform = translation(1.5,0.5,-0.5) * scaling(0.5,0.5,0.5); 
    right->mat = Material(); 
    right->mat.diffuse = 0.7; 
    right->mat.mat_color = Color(0.5,1,0.1); 
    right->mat.specular = 0.3; 

    Sphere* left = new Sphere(); 
    left->transform = translation(-1.5,0.33,-0.75) * scaling(0.33,0.33,0.33); 
    left->mat = Material(); 
    left->mat.diffuse = 0.7; 
    left->mat.mat_color = Color(1,0.8,0.1); 
    left->mat.specular = 0.3; 

    World w; 
    w.world_light.position = Point(10,10,-10); 

    for(Shape* s: w.world_objects)
    {
        delete s; 
    }

    w.world_objects.clear(); 

    w.world_objects.push_back(wall); 
    w.world_objects.push_back(floor); 
    w.world_objects.push_back(left); 
    w.world_objects.push_back(middle); 
    w.world_objects.push_back(right); 

    Camera cam(600,400,M_PI/3.f);
    cam.transform = view_transform(Point(0,1.5,-5),Point(0,1,0),Vector(0,1,0));  

    Canvas image = render(cam,w); 

    image.CanvasToPPM("demo4.ppm"); 
}