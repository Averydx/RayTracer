#include <catch2/catch_test_macros.hpp>

#include "camera.h"
#include "tuple.h"
#include "point.h"
#include "color.h"
#include "vector.h"
#include "tools.h"
#include "canvas.h"
#include "matrix.h"
#include "transformations.h"
#include "ray.h"
#include "shapes.h"
#include "lights.h"
#include "materials.h"
#include "world.h"
#include "intersection.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>

TEST_CASE("Intersect a world with a ray","[world]")
{
    World w; 
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    std::vector<Intersection> xs = w.intersect(r); 

    REQUIRE(xs.size() == 4); 
    REQUIRE(equal_double(xs[0].t,4)); 
    REQUIRE(equal_double(xs[1].t,4.5)); 
    REQUIRE(equal_double(xs[2].t,5.5)); 
    REQUIRE(equal_double(xs[3].t,6.0)); 
}

TEST_CASE("Precomputing the state of an intersection","[computations]")
{
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    Sphere s; 
    Intersection I(4,&s); 

    Computations comps(I,r); 

    REQUIRE(equal_double(comps.t,I.t)); 
    REQUIRE(comps.point == Point(0,0,-1)); 
    REQUIRE(comps.eyev == Vector(0,0,-1)); 
    REQUIRE(comps.normalv == Vector(0,0,-1)); 

}

TEST_CASE("Checking whether hit is inside or outside","[computations]")
{
    SECTION("Outside")
    {
        Ray r(Point(0,0,-5),Vector(0,0,1)); 
        Sphere s; 

        Intersection I(4,&s) ; 
        Computations comps(I,r); 

        REQUIRE(comps.inside == false); 
    }
    SECTION("Inside")
    {
        Ray r(Point(0,0,0),Vector(0,0,1)); 
        Sphere s; 

        Intersection I(1,&s) ; 
        Computations comps(I,r); 

        REQUIRE(comps.point == Point(0,0,1)); 
        REQUIRE(comps.eyev == Vector(0,0,-1)); 
        REQUIRE(comps.inside == true); 
        //inverted normal
        REQUIRE(comps.normalv == Vector(0,0,-1)); 
    }
    
}

TEST_CASE("Precomputing the reflection vector","[computations]")
{
    Plane shape; 
    Ray r(Point(0,1,-1),Vector(0,-sqrt(2)/2.f,sqrt(2)/2.f)); 
    Intersection I(sqrt(2),&shape); 
    Computations comps(I,r); 
    REQUIRE(comps.reflectv == Vector(0,sqrt(2)/2.f,sqrt(2)/2.f)); 
}

TEST_CASE("Point colors", "[world]")
{
    World w; 

    SECTION("The color when a ray misses")
    {
        Ray r(Point(0,0,-5),Vector(0,1,0)); 
        Color c = w.color_at(r); 

        REQUIRE(c == Color(0,0,0)); 
    }

    SECTION("The color when a ray hits")
    {
        Ray r(Point(0,0,-5),Vector(0,0,1)); 
        Color c = w.color_at(r); 

        REQUIRE(c == Color(0.268071,0.335089,0.201053)); 
    }

    SECTION("The color with an intersection behind the ray")
    {
        Shape* outer = w.world_objects[0]; 
        outer->mat = Material(1.0,0.7,0.2,200.,Color(0.8,1.0,0.6)); 

        Shape* inner = w.world_objects[1]; 
        inner->mat = Material(1.0,0.7,0.2,200.,Color(0.8,1.0,0.6));

        Ray r(Point(0,0,.75),Vector(0,0,-1)); 

        Color c = w.color_at(r); 

        REQUIRE(c == inner->mat.mat_color); 
    }
}

TEST_CASE("Constructing a camera","[camera]")
{
    int hsize = 160; 
    int vsize = 120; 
    double field_of_view = M_PI/2.f; 
    
    Camera c(hsize,vsize,field_of_view); 

    REQUIRE(c.hsize == 160); 
    REQUIRE(c.vsize == 120); 
    REQUIRE(c.field_of_view == M_PI/2.f); 
    Matrix I(4,4); 
    I.setIdentity(); 
    REQUIRE(c.transform == I); 
}

TEST_CASE("Computing pixel size","[camera]")
{
    SECTION("The pixel size for a horizontal canvas")
    {
        Camera c(200,125,M_PI/2.f); 
        REQUIRE(equal_double(c.pixel_size,0.01)); 
    }

    SECTION("The pixel size for a vertical canvas")
    {
        Camera c(125,200,M_PI/2.f); 
        REQUIRE(equal_double(c.pixel_size,0.01)); 
    }
}

TEST_CASE("Constructing rays through the canvas","[camera]")
{
    Camera c(201,101,M_PI/2.f); 
    SECTION("Constructing a ray through the center of the canvas")
    {
        Ray r = c.ray_for_pixel(100,50); 
        REQUIRE(r.origin == Point(0,0,0)); 
        REQUIRE(r.direction == Vector(0,0,-1)); 
    }
    SECTION("Constructing a ray through the corner of the canvas")
    {
        Ray r = c.ray_for_pixel(0,0); 
        REQUIRE(r.origin == Point(0,0,0)); 
        REQUIRE(r.direction == Vector(0.66519,0.33259,-0.66851)); 
    }
    SECTION("Constructing a ray when the camera is transformed")
    {
        c.transform = rotation_y(M_PI/4.f) * translation(0,-2,5); 
        Ray r = c.ray_for_pixel(100,50); 
        REQUIRE(r.origin == Point(0,2,-5)); 
        REQUIRE(r.direction == Vector(sqrt(2)/2.f,0.,-sqrt(2)/2.f)); 
    }
}
 