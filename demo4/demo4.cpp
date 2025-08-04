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
#include "parser.h"

#include <vector> 
#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>


int main(int argc, char *argv[])
{
    
    Plane* floor = new Plane(); 
    floor->mat.pattern = new CheckerPattern(Color(1.f,1.f,1.f),Color(0.f,0.f,0.f));
    floor->mat.pattern->transform = scaling(0.33,0.33,0.33); 

    // Plane* wall1 = new Plane(); 
    // wall1->mat.mat_color = Color(0.768,0.768,0.768); 
    // wall1->transform = (translation(0,0,10)*rotation_x(M_PI/2.f)); 
    // wall1->mat.reflective = 1.0; 

    Group* walls = new Group(); 
    //walls->add_child(wall1); 
    walls->add_child(floor); 

    World w; 
    w.world_light.position = Point(20,20,-20); 
    w.empty_objects(); 

    Parser p("C:/Users/avery/OneDrive/Desktop/RayTracer/models/CopperPot.obj"); 
    p.read_file();
    
    std::cout<<"Vertices in file: "<<p.vertices.size()<<std::endl; 

    p.default_group->transform = translation(0,0,2) * scaling(0.2,0.2,0.2); 
    // p.default_group->mat.transparency = 0.; 
    p.default_group->mat.reflective = 0.3; 
    // p.default_group->mat.refractive_index = 1.; 
    p.default_group->mat.mat_color = Color(0.72, 0.45, 0.20);  
    p.default_group->percolate_material(); 

    std::cout<<"triangle count: "<<p.default_group->children.size()<<std::endl; 

    Parser p2("C:/Users/avery/OneDrive/Desktop/RayTracer/models/glassBottle.obj"); 
    p2.read_file(); 

    p2.default_group->transform = translation(0,1.25,-1) * scaling(0.1,0.1,0.1);  
    p2.default_group->mat.transparency = 0.9; 
    p2.default_group->mat.reflective = 1.0; 
    p2.default_group->mat.refractive_index = 1.5; 
    p2.default_group->mat.mat_color = Color(0.2, 0.2, 0.2);  
    p2.default_group->percolate_material(); 

    Group* scene_group = new Group(); 
    scene_group->add_child(p.default_group); 
    scene_group->add_child(walls); 
    scene_group->add_child(p2.default_group); 

    scene_group->refresh_bvh(); 

    w.add_object(scene_group); 
    
    Camera cam(1920,1080,M_PI/3.f);
    cam.transform = view_transform(Point(0,2,-7),Point(0,2,0),Vector(0,1,0));  


    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    Canvas image = render(cam,w); 
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Render time: " << elapsed_seconds.count() << " seconds" << std::endl;

    image.CanvasToPPM("demo4.ppm"); 
}