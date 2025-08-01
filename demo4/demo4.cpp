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
#include "bvh.h"

#include <vector> 
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <chrono>


int main(int argc, char *argv[])
{

    std::mt19937 gen(0);
    std::uniform_real_distribution<> distrib_1(0, 1);
    std::uniform_real_distribution<> distrib_2(-5, 5);

    Plane* floor = new Plane(); 
    floor->mat.pattern = new CheckerPattern(Color(1.f,1.f,1.f),Color(0.f,0.f,0.f));
    floor->mat.pattern->transform = scaling(0.33,0.33,0.33); 

    Plane* wall1 = new Plane(); 
    wall1->mat.mat_color = Color(0.768,0.768,0.768); 
    wall1->mat.reflective = 1.0; 
    wall1->setTransform(translation(0,0,10)*rotation_x(M_PI/2.f)); 

    // Cube* cube = new Cube(); 
    // cube->transform = translation(-3,1,0); 
    // cube->mat.reflective = 0.3; 
    // cube->mat.refractive_index = 1.2; 
    // cube->mat.transparency = 0.2; 
    // cube->mat.mat_color = Color(0.3,0.1,0.8); 

    // Cylinder* cyl = new Cylinder(0,2,CYL_TYPE::CLOSED); 
    // cyl->mat.transparency = 0.5; 
    // cyl->mat.refractive_index = 1.3; 
    // cyl->mat.reflective = 0.3; 

    World w; 
    w.world_light.position = Point(20,20,-20); 
    w.empty_objects(); 

    std::vector<Point> positions = {Point(0,1,0),Point(-2,1,2),Point(2,1,3),Point(0,1,4),Point(-5,1,4),Point(5,1,4),Point(1,1,-2),Point(1,1,2),Point(-3,1,0)}; 
    for(int i = 0; i < positions.size(); i++)
    {
        Sphere* ball = glass_sphere(); 
        ball->transform = translation(positions[i].x,positions[i].y,positions[i].z); 
        ball->mat.mat_color = Color(distrib_1(gen),distrib_1(gen),distrib_1(gen)); 
        w.world_objects.push_back(ball); 
    }

    w.world_objects.push_back(wall1); 
    w.world_objects.push_back(floor);  
    // w.world_objects.push_back(cube); 
    // w.world_objects.push_back(cyl); 

    BVHNode* bvh = build_bvh(w.world_objects,2); 
    std::cout<<count_bvh(bvh)<<std::endl; 
    w.bvh = bvh; 

    print_bvh_stats(bvh); 

    Camera cam(1920,1080,M_PI/3.f);
    cam.transform = view_transform(Point(0,3,-6),Point(0,1,0),Vector(0,1,0));  

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    Canvas image = render(cam,w); 
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Render time: " << elapsed_seconds.count() << " seconds" << std::endl;

    image.CanvasToPPM("demo4.ppm"); 
}