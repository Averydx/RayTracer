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
#include "shapes.h"
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

TEST_CASE("Reflectivity for the default material","[materials]")
{
    Material m; 
    REQUIRE(equal_double(m.reflective,0.f)); 
}

TEST_CASE("Creating a stripe pattern","[pattern][materials]")
{
    StripePattern pattern(Color(1,1,1),Color(0,0,0)); 
    SECTION("Constructor")
    {
        REQUIRE(pattern.ca == Color(1,1,1)); 
        REQUIRE(pattern.cb == Color(0,0,0)); 
    }
    SECTION("A stripe pattern is constant in z")
    {
        REQUIRE(pattern.color_at(Point(0,0,0)) == Color(1,1,1)); 
        REQUIRE(pattern.color_at(Point(0,0,1)) == Color(1,1,1)); 
        REQUIRE(pattern.color_at(Point(0,0,2)) == Color(1,1,1)); 
    }
    SECTION("A stripe pattern is constant in y")
    {
        REQUIRE(pattern.color_at(Point(0,0,0)) == Color(1,1,1)); 
        REQUIRE(pattern.color_at(Point(0,1,0)) == Color(1,1,1)); 
        REQUIRE(pattern.color_at(Point(0,2,0)) == Color(1,1,1)); 
    }
        SECTION("A stripe pattern is alternates in x")
    {
        REQUIRE(pattern.color_at(Point(0,0,0)) == Color(1,1,1)); 
        REQUIRE(pattern.color_at(Point(0.9,0,0)) == Color(1,1,1)); 
        REQUIRE(pattern.color_at(Point(1,0,0)) == Color(0,0,0)); 
        REQUIRE(pattern.color_at(Point(-0.1,0,0)) == Color(0,0,0)); 
        REQUIRE(pattern.color_at(Point(-1.0,0,0)) == Color(0,0,0)); 
        REQUIRE(pattern.color_at(Point(-1.1,0,0)) == Color(1,1,1)); 
    }
}
TEST_CASE("Lighting with a pattern applied","[lighting][pattern][materials]")
{
    Sphere s; 
    Material m; 
    m.pattern = new StripePattern(Color(1,1,1),Color(0,0,0)); 
    m.ambient = 1; 
    m.diffuse = 0; 
    m.specular = 0; 

    Vector eyev(0,0,-1); 
    Vector normalv(0,0,-1);
    pointLight light(Color(1,1,1),Point(0,0,-10)); 
    
    Color c1 = lighting(m,&s, light,Point(0.9,0,0),eyev,normalv); 
    Color c2 = lighting(m,&s, light,Point(1.1,0,0),eyev,normalv); 

    REQUIRE(c1 == Color(1,1,1)); 
    REQUIRE(c2 == Color(0,0,0)); 
}
TEST_CASE("Patterns and transformations","[pattern]")
{
    SECTION("Stripes with an object transformation","[pattern]")
    {
        Sphere s; 
        s.transform = scaling(2,2,2); 
        StripePattern pattern(Color(1,1,1),Color(0,0,0)); 
        Color c = pattern.color_at_object(&s,Point(1.5,0,0)); 

        REQUIRE(c == Color(1,1,1)); 
    }
    SECTION("Stripes with a pattern transformation")
    {
        Sphere s; 
        StripePattern pattern(Color(1,1,1),Color(0,0,0)); 
        pattern.transform = scaling(2,2,2); 
        Color c = pattern.color_at_object(&s,Point(1.5,0,0)); 

        REQUIRE(c == Color(1,1,1)); 
    }
    SECTION("Stripes with both an object and a pattern transformation")
    {
        Sphere s; 
        s.transform = scaling(2,2,2); 
        StripePattern pattern(Color(1,1,1),Color(0,0,0)); 
        pattern.transform = translation(0.5,0,0); 
        Color c = pattern.color_at_object(&s,Point(2.5,0,0)); 

        REQUIRE(c == Color(1,1,1)); 
    }
}

//Need to fix
// TEST_CASE("A gradient linearly interpolates between colors","[pattern]")
// {
//     GradientPattern pattern(Color(1.f,1.f,1.f),Color(0.f,0.f,0.f)); 

//     REQUIRE(pattern.color_at(Point(0.f,0.f,0.f)) == Color(1.f,1.f,1.f)); 
//     REQUIRE(pattern.color_at(Point(0.25,0.f,0.f)) == Color(0.75,0.75,0.75)); 
//     REQUIRE(pattern.color_at(Point(0.5,0.f,0.f)) == Color(0.5,0.5,0.5)); 
//     REQUIRE(pattern.color_at(Point(0.75,0.f,0.f)) == Color(0.25,0.25,0.25)); 
// }

TEST_CASE("A ring should extend in both x and z","[pattern]")
{
    RingPattern pattern(Color(1.f,1.f,1.f),Color(0.f,0.f,0.f)); 

    REQUIRE(pattern.color_at(Point(0.f,0.f,0.f)) == Color(1.f,1.f,1.f)); 
    REQUIRE(pattern.color_at(Point(1.0,0.f,0.f)) == Color(0.,0.,0.)); 
    REQUIRE(pattern.color_at(Point(0.,0.f,1.f)) == Color(0.,0.,0.)); 
    REQUIRE(pattern.color_at(Point(0.708,0.f,0.708f)) == Color(0.,0.,0.)); 
}

TEST_CASE("Checker pattern","[pattern]")
{
    CheckerPattern pattern(Color(1.f,1.f,1.f),Color(0.f,0.f,0.f)); 
    SECTION("Checkers should repeat in x")
    {
        REQUIRE(pattern.color_at(Point(0.f,0.f,0.f)) == Color(1.f,1.f,1.f)); 
        REQUIRE(pattern.color_at(Point(0.99,0.f,0.f)) == Color(1.,1.,1.)); 
        REQUIRE(pattern.color_at(Point(1.01,0.f,0.f)) == Color(0.,0.,0.)); 
    }

    SECTION("Checkers should repeat in y")
    {
        REQUIRE(pattern.color_at(Point(0.f,0.f,0.f)) == Color(1.f,1.f,1.f)); 
        REQUIRE(pattern.color_at(Point(0.0,0.99f,0.f)) == Color(1.,1.,1.)); 
        REQUIRE(pattern.color_at(Point(0.f,1.01f,0.f)) == Color(0.,0.,0.)); 
    }

    SECTION("Checkers should repeat in z")
    {
        REQUIRE(pattern.color_at(Point(0.f,0.f,0.f)) == Color(1.f,1.f,1.f)); 
        REQUIRE(pattern.color_at(Point(0.,0.f,0.99f)) == Color(1.,1.,1.)); 
        REQUIRE(pattern.color_at(Point(0,0.f,1.01f)) == Color(0.,0.,0.)); 
    }
}




TEST_CASE("Lighting tests","[lighting]")
{
    Sphere s; 
    Material m; 
    Point pos(0.f,0.f,0.f); 

    SECTION("Lighting with the eye between the light and the surface")
    {
        Vector eyev(0,0,-1); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,-10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(1.9,1.9,1.9)); 
    }

    SECTION("Light with the eye between light and surface, eye offset 45deg","[lighting]")
    {
        Vector eyev(0,sqrt(2)/2.f,-sqrt(2)/2.f); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,-10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(1.0,1.0,1.0)); 
    }

    SECTION("Lighting with eye opposite surface, light offset 45deg","[lighting]")
    {
        Vector eyev(0,0,-1); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,10,-10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(0.7364,0.7364,0.7364)); 
    }

    SECTION("Lighting with eye in the path of the reflection vector","[lighting]")
    {
        Vector eyev(0,-sqrt(2)/2.f,-sqrt(2)/2.f); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,10,-10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(1.6364,1.6364,1.6364)); 
    }

    SECTION("Lighting with the light behind the surface","[lighting]")
    {
        Vector eyev(0,0,-1);  
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(0.1,0.1,0.1)); 
    }
    SECTION("Lighting with the surface in shadow","[lighting]")
    {
        Vector eyev(0,0,-1); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,-10)); 
        bool in_shadow = true; 

        Color result = lighting(m,&s,light,pos,eyev,normalv,in_shadow); 

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

TEST_CASE("Reflective surfaces","[lighting]")
{
    World w; 
    SECTION("The reflected color for a nonreflective material")
    {
        Ray r(Point(0,0,0),Vector(0,0,1)); 
        w.world_objects[1]->mat.ambient = 1.; 
        Intersection I(1,w.world_objects[1]); 
        Computations comps(I,r); 

        Color color = w.reflected_color(comps); 
        REQUIRE(color == Color(0,0,0)); 
    }

    SECTION("The reflected color for a reflective material")
    {
        Plane* shape = new Plane(); 
        shape->mat.reflective = 0.5; 
        shape->transform = translation(0,-1,0); 
        w.world_objects.push_back(shape); 

        Ray r(Point(0,0,-3),Vector(0,-sqrt(2)/2.f,sqrt(2)/2.f)); 
        Intersection I(sqrt(2),shape); 
        Computations comps(I,r); 

        Color color = w.reflected_color(comps); 
        REQUIRE(color == Color(0.19032,0.2379,0.14274)); 


    }
}

TEST_CASE("shade_hit() with a reflective material","[lighting]")
{
    World w; 
    Plane* shape = new Plane();
    shape->mat.reflective = 0.5; 
    shape->transform = translation(0,-1,0); 
    w.world_objects.push_back(shape); 

    Ray r(Point(0,0,-3),Vector(0,-sqrt(2)/2.f,sqrt(2)/2.f)); 
    Intersection I(sqrt(2),shape);

    Computations comps(I,r); 
    Color color = w.shade_hit(comps); 

    REQUIRE(color == Color(0.87677,0.92436,0.82918)); 
}

