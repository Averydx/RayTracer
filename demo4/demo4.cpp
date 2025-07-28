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
    floor->mat.pattern = new CheckerPattern(Color(1.f,1.f,1.f),Color(0.f,0.f,0.f));
    floor->mat.reflective = 0.5f;  

    Plane* wall1 = new Plane(); 
    wall1->mat.mat_color = Color(0.5,0.5,0.5); 
    wall1->mat.reflective = 1.f;
    wall1->mat.transparency = 0.0;  
    wall1->setTransform(translation(0,0,2.5)*rotation_x(M_PI/2.f)); 

    Sphere* middle = new Sphere();
    middle->transform = translation(0,0.0,-0.5);   
    middle->mat = Material(); 
    middle->mat.mat_color = Color(0.5,0.5,0.5); 
    middle->mat.diffuse = 0.7; 
    middle->mat.specular = 0.3; 
    middle->mat.reflective = 0.5f; 
    middle->mat.refractive_index = 1.5; 
    middle->mat.transparency = 0.3; 

    Sphere* right = new Sphere(); 
    right->transform = translation(1.5,0.5,-0.5) * scaling(0.5,0.5,0.5); 
    right->mat = Material(); 
    right->mat.pattern = new GradientPattern(Color(1.f,0.f,1.f),Color(0.f,0.5f,0.f)); 
    right->mat.pattern->transform = scaling(0.3,0.3,0.3); 
    right->mat.diffuse = 0.7; 
    right->mat.specular = 0.3; 
    right->mat.reflective = 0.5f; 
    right->mat.refractive_index = 1.5; 
    right->mat.transparency = 0.3; 

    Sphere* left = new Sphere(); 
    left->transform = translation(-1.5,0.33,-0.75) * scaling(0.33,0.33,0.33); 
    left->mat = Material(); 
    left->mat.diffuse = 0.7; 
    left->mat.mat_color = Color(1,0.8,0.1); 
    left->mat.specular = 0.3; 
    left->mat.reflective = 1.0f; 
    left->mat.refractive_index = 1.5; 
    left->mat.transparency = 0.6; 

    World w; 
    w.world_light.position = Point(10,10,-10); 
    w.empty_objects(); 

    w.world_objects.push_back(wall1); 
    w.world_objects.push_back(floor); 
    w.world_objects.push_back(left); 
    w.world_objects.push_back(middle); 
    w.world_objects.push_back(right); 

    Camera cam(600,400,M_PI/3.f);
    cam.transform = view_transform(Point(0,1.5,-5),Point(0,1,0),Vector(0,1,0));  

    Canvas image = render(cam,w); 

    image.CanvasToPPM("demo4.ppm"); 
}