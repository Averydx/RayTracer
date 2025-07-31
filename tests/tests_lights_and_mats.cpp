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

    REQUIRE(c1 == Color(0.704225,0.704225,0.704225)); 
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

        REQUIRE(result == Color(1.33803,1.33803,1.33803)); 
    }

    SECTION("Light with the eye between light and surface, eye offset 45deg","[lighting]")
    {
        Vector eyev(0,sqrt(2)/2.f,-sqrt(2)/2.f); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,-10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(0.704225,0.704225,0.704225)); 
    }

    SECTION("Lighting with eye opposite surface, light offset 45deg","[lighting]")
    {
        Vector eyev(0,0,-1); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,10,-10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(0.518589,0.518589,0.518589)); 
    }

    SECTION("Lighting with eye in the path of the reflection vector","[lighting]")
    {
        Vector eyev(0,-sqrt(2)/2.f,-sqrt(2)/2.f); 
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,10,-10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(1.15239,1.15239,1.15239)); 
    }

    SECTION("Lighting with the light behind the surface","[lighting]")
    {
        Vector eyev(0,0,-1);  
        Vector normalv(0,0,-1); 
        pointLight light(Color(1,1,1),Point(0,0,10)); 
        Color result = lighting(m,&s,light,pos,eyev,normalv); 

        REQUIRE(result == Color(0.0704225,0.0704225,0.0704225)); 
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

    REQUIRE(comps.over_point.z < - BUMP_EPSILON/2.f); 
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

        REQUIRE(color == Color(0.134157,0.167696,0.100618)); 


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


    REQUIRE(color == Color( 0.617534,0.651073,0.583994)); 
}

//This needs more thought
// TEST_CASE("color_at() with mutually reflective surfaces","[lighting]")
// {
//     World w; 
//     w.world_light = pointLight(Point(0,0,0),Color(1,1,1)); 
//     Plane* lower = new Plane(); 
//     lower->mat.reflective = 1; 
//     lower->transform = translation(0,-1,0); 
//     w.world_objects.push_back(lower); 

//     Plane* upper = new Plane(); 
//     upper->mat.reflective = 1; 
//     upper->transform = translation(0,1,0); 
//     w.world_objects.push_back(upper);
    
//     Ray r(Point(0,0,0),Vector(0,1,0)); 

//     Color c = w->color_at(r); 
// }

TEST_CASE("The refracted color with a refracted ray","[refraction]")
{
    World w; 
    w.world_objects[0]->mat.ambient = 1.0; 
    w.world_objects[0]->mat.pattern = new TestPattern(); 

    w.world_objects[1]->mat.transparency = 1.0; 
    w.world_objects[1]->mat.refractive_index = 1.5; 

    Ray r(Point(0,0,0.1),Vector(0,1,0)); 
    std::vector<Intersection> xs = intersections({
        Intersection(-0.9899,w.world_objects[0]),
        Intersection(-0.4899,w.world_objects[1]),
        Intersection(0.4899,w.world_objects[1]),
        Intersection(0.9899,w.world_objects[0])
    }); 

    Computations comps(xs[2],r,xs); 
    Color c = w.refracted_color(comps,5); 
    REQUIRE(c == Color(0,0.99888,0.04725)); 
}

TEST_CASE("The reflected color at the maximum recursive depth","[lighting]")
{
    World w; 
    Plane* shape = new Plane();
    shape->mat.reflective = 0.5; 
    shape->transform = translation(0,-1,0); 
    w.world_objects.push_back(shape); 

    Ray r(Point(0,0,-3),Vector(0,-sqrt(2)/2.f,sqrt(2)/2.f)); 
    Intersection I(sqrt(2),shape);

    Computations comps(I,r); 
    Color color = w.reflected_color(comps,0); 

    REQUIRE(color == Color(0.0,0.0,0.0)); 
}

TEST_CASE("Transparency and Refractive Index for the default material","[materials][refraction]")
{
    Material m; 
    REQUIRE(equal_double(m.transparency,0.f)); 
    REQUIRE(equal_double(m.refractive_index,1.f)); 
}

TEST_CASE("Finding n1 and n2 at various intersections","[materials][refraction]")
{
    std::vector<double> n1s = {1.0,1.5,2.0,2.5,2.5,1.5}; 
    std::vector<double> n2s = {1.5,2.0,2.5,2.5,1.5,1.0}; 

    Sphere* A = glass_sphere(); 
    A->transform = scaling(2,2,2); 
    A->mat.refractive_index = 1.5; 

    Sphere* B = glass_sphere(); 
    B->transform = translation(0,0,-0.25); 
    B->mat.refractive_index = 2; 

    Sphere* C = glass_sphere(); 
    C->transform = translation(0,0,0.25); 
    C->mat.refractive_index = 2.5;
    
    Ray r(Point(0,0,-4),Vector(0,0,1)); 
    std::vector<Intersection> xs = intersections({Intersection(2,A),Intersection(2.75,B),Intersection(3.25,C),Intersection(4.75,B),Intersection(5.25,C),Intersection(6,A)}); 

    int counter = 0; 
    for(const Intersection& I: xs)
    {
        Computations comps(I,r,xs); 
        REQUIRE(equal_double(comps.n1,n1s[counter])); 
        REQUIRE(equal_double(comps.n2,n2s[counter])); 
        counter++; 
    }
}

TEST_CASE("The under point is offset below the surface","[refraction]")
{
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    Sphere* shape = glass_sphere();
    shape->transform = translation(0,0,1); 
    Intersection I(5,shape); 
    std::vector<Intersection> xs = intersections({I}); 

    Computations comps(I,r,xs); 
    REQUIRE(comps.under_point.z > BUMP_EPSILON / 2); 
}

TEST_CASE("The refracted color with an opaque surface","[refraction]")
{
    World w; 
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    std::vector<Intersection> xs = intersections({Intersection(4,w.world_objects[0]),Intersection(6,w.world_objects[0])}); 
    Computations comps(xs[0],r,xs); 
    Color c = w.refracted_color(comps,5); 
    REQUIRE(c == Color(0,0,0)); 
}

TEST_CASE("The refracted color at the maximum recursive depth","[refraction]")
{
    World w; 
    w.world_objects[0]->mat.transparency = 1.f; 
    w.world_objects[0]->mat.refractive_index = 1.5f; 
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    std::vector<Intersection> xs = intersections({Intersection(4,w.world_objects[0]),Intersection(6,w.world_objects[0])}); 
    Computations comps(xs[0],r,xs); 
    Color c = w.refracted_color(comps,0); 
    REQUIRE(c == Color(0,0,0)); 
}
TEST_CASE("The refracted color under total internal reflection","[refraction]")
{
    World w; 
    w.world_objects[0]->mat.transparency = 1.f; 
    w.world_objects[0]->mat.refractive_index = 1.5f; 
    Ray r(Point(0,0,sqrt(2)/2),Vector(0,1,0)); 
    std::vector<Intersection> xs = intersections({Intersection(-sqrt(2)/2,w.world_objects[0]),Intersection(sqrt(2)/2,w.world_objects[0])}); 
    //This time we are inside the sphere so we need to look at the second intersection xs[1]
    Computations comps(xs[1],r,xs); 
    Color c = w.refracted_color(comps,5); 
    REQUIRE(c == Color(0,0,0)); 
}

TEST_CASE("The schlick approximation under total internal reflection","[refraction]")
{
    World w; 
    Shape* shape = glass_sphere(); 
    Ray r(Point(0,0,sqrt(2)/2.f),Vector(0,1,0)); 
    std::vector<Intersection> xs = intersections({Intersection(-sqrt(2)/2.f,shape),Intersection(sqrt(2)/2.f,shape)}); 
    Computations comps(xs[1],r,xs); 
    double reflectance = w.schlick(comps); 
    REQUIRE(equal_double(reflectance,1.f)); 
}

TEST_CASE("The schlick approximation with a perpendicular viewing angle","[refraction]")
{
    World w; 
    Shape* shape = glass_sphere(); 
    Ray r(Point(0,0,0),Vector(0,1,0)); 
    std::vector<Intersection> xs = intersections({Intersection(-1,shape),Intersection(1,shape)}); 
    Computations comps(xs[1],r,xs); 
    double reflectance = w.schlick(comps); 
    REQUIRE(equal_double(reflectance,0.04f)); 
}

TEST_CASE("The schlick approximation with small angle and n2 > n1","[refraction]")
{
    World w; 
    Shape* shape = glass_sphere(); 
    Ray r(Point(0,0.99,-2),Vector(0,0,1)); 
    std::vector<Intersection> xs = intersections({Intersection(1.8589,shape)}); 
    Computations comps(xs[0],r,xs); 
    double reflectance = w.schlick(comps); 
    REQUIRE(equal_double(reflectance,0.48873f)); 
}

TEST_CASE("shade_hit() with a transparent material","[material][refraction]")
{
    World w; 
    w.empty_objects(); 

    Plane* floor = new Plane(); 
    floor->transform = translation(0,-1,0); 
    floor->mat.transparency = 0.5; 
    floor->mat.refractive_index = 1.5; 

    Sphere* ball = new Sphere(); 
    ball->mat.mat_color = Color(1,0,0); 
    ball->mat.ambient = 0.5; 
    ball->transform = translation(0,-3.5,-0.5); 

    w.world_objects.push_back(floor); 
    w.world_objects.push_back(ball); 

    Ray r(Point(0,0,-3),Vector(0,-sqrt(2)/2.f,sqrt(2)/2.f)); 
    std::vector<Intersection> xs = intersections({Intersection(sqrt(2),floor)}); 
    Computations comps(xs[0],r,xs); 
    Color color = w.shade_hit(comps,5); 

    REQUIRE(color == Color(0.733398,0.483398,0.483398)); 
}

TEST_CASE("shade_hit() with a reflective, transparent material","[refraction]")
{
    World w; 
    w.empty_objects(); 
    Ray r(Point(0,0,-3),Vector(0,-sqrt(2)/2.0,sqrt(2)/2.0)); 
    Plane* floor = new Plane(); 
    floor->transform = translation(0,-1,0); 
    floor->mat.reflective = 0.5; 
    floor->mat.transparency = 0.5; 
    floor->mat.refractive_index = 1.5; 
    w.world_objects.push_back(floor); 

    Sphere* ball = new Sphere(); 
    ball->mat.mat_color = Color(1,0,0); 
    ball->mat.ambient = 0.5; 
    ball->transform = translation(0,-3.5,-0.5); 
    w.world_objects.push_back(ball); 
    std::vector<Intersection> xs = intersections({Intersection(sqrt(2),floor)}); 
    Computations comps(xs[0],r,xs); 
    Color color = w.shade_hit(comps,5); 

    // Color difference = Color(0.93391,0.69643,0.69243) - color; 
    // difference.printColor(); 
    // color.printColor(); 

    REQUIRE(color == Color(0.722881, 0.483398,0.483398)); 
}

