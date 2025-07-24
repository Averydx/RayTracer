#include <catch2/catch_test_macros.hpp>

#include "tuple.h"
#include "point.h"
#include "color.h"
#include "vector.h"
#include "tools.h"
#include "canvas.h"
#include "matrix.h"
#include "transformations.h"
#include "ray.h"
#include "sphere.h"
#include "lights.h"
#include "materials.h"
#include "world.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>

TEST_CASE("Generating a point light","[lights]")
{
    Color light_intensity(1,1,1); 
    Point light_pos(0,0,0); 

    pointLight light(light_intensity,light_pos); 

    REQUIRE(light.intensity == light_intensity); 
    REQUIRE(light.position == light_pos); 
}

TEST_CASE("Generating a material","[materials]")
{
    Material m; 

    REQUIRE(m.mat_color == Color(1,1,1)); 
    REQUIRE(equal_double(m.ambient,0.1)); 
    REQUIRE(equal_double(m.diffuse,0.9));
    REQUIRE(equal_double(m.specular,0.9));
    REQUIRE(equal_double(m.shininess,200.0));
}

TEST_CASE("Lighting tests","[lighting]")
{
    Material m; 
    Point pos(0.f,0.f,0.f); 

    SECTION("Lighting with the eye between the light and the surface")
    {
        Vector eyev(0,0,-1); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,-10)); 
        Color result = lighting(m,light,pos,eyev,normalv); 

        REQUIRE(result == Color(1.9,1.9,1.9)); 
    }

    SECTION("Light with the eye between light and surface, eye offset 45deg","[lighting]")
    {
        Vector eyev(0,sqrt(2)/2.f,-sqrt(2)/2.f); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,-10)); 
        Color result = lighting(m,light,pos,eyev,normalv); 

        REQUIRE(result == Color(1.0,1.0,1.0)); 
    }

    SECTION("Lighting with eye opposite surface, light offset 45deg","[lighting]")
    {
        Vector eyev(0,0,-1); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,10,-10)); 
        Color result = lighting(m,light,pos,eyev,normalv); 

        REQUIRE(result == Color(0.7364,0.7364,0.7364)); 
    }

    SECTION("Lighting with eye in the path of the reflection vector","[lighting]")
    {
        Vector eyev(0,-sqrt(2)/2.f,-sqrt(2)/2.f); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,10,-10)); 
        Color result = lighting(m,light,pos,eyev,normalv); 

        REQUIRE(result == Color(1.6364,1.6364,1.6364)); 
    }

    SECTION("Lighting with the light behind the surface","[lighting]")
    {
        Vector eyev(0,0,-1);  
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,10)); 
        Color result = lighting(m,light,pos,eyev,normalv); 

        REQUIRE(result == Color(0.1,0.1,0.1)); 
    }
    SECTION("Lighting with the surface in shadow","[lighting]")
    {
        Vector eyev(0,0,-1); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,-10)); 
        bool in_shadow = true; 

        Color result = lighting(m,light,pos,eyev,normalv,in_shadow); 

        REQUIRE(result == Color(0.1,0.1,0.1)); 
    }
}
TEST_CASE("Shadow logic","[lighting][world]")
{
    World w; 
    SECTION("There is no shadow when nothing is collinear with point and light")
    {
        Point p(0,10,0); 
        REQUIRE(!w.is_shadowed(p)); 
    }
    SECTION("The shadow when an object is between the point and the light")
    {
        Point p(10,-10,10); 
        REQUIRE(w.is_shadowed(p)); 
    }
    SECTION("There is no shadow when the object is behind the point")
    {
        Point p(-2,2,-2); 
        REQUIRE(!w.is_shadowed(p)); 
    }
    SECTION("There is no shadow when an object is behind the light")
    {
        Point p(-20,20,-20); 
        REQUIRE(!w.is_shadowed(p)); 
    }
}

TEST_CASE("shade_hit() is given an intersection in shadow","[lighting][world]")
{
    World w;
    w.world_light = pointLight(Color(1,1,1),Point(0,0,-10)); 
    
    w.world_objects[1]->transform = translation(0,0,10); 

    Ray r(Point(0,0,5),Vector(0,0,1)); 
    Intersection I(4,w.world_objects[1]); 

    Computations comps(I,r); 

    Color c = w.shade_hit(comps); 

    REQUIRE(c == Color(0.1,0.1,0.1)); 
}

TEST_CASE("The hit should offset the point","[lighting]")
{
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    Sphere s; 
    s.transform = translation(0,0,1); 
    Intersection I(5,&s); 

    Computations comps(I,r); 

    REQUIRE(comps.over_point.z < - EPSILON/2.f); 
    REQUIRE(comps.point.z > comps.over_point.z);
}


