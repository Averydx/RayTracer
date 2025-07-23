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
#include "materials.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>

TEST_CASE("Creating a sphere","[sphere]")
{
    Sphere s;
    Matrix I(4,4); 
    I.setIdentity(); 
    REQUIRE(s.getTransform() == I); 
}

TEST_CASE("Sphere default transform","[sphere]")
{
    Sphere s;
    Matrix m(4,4); 
    m.setIdentity(); 
    REQUIRE(s.getTransform() == m); 
}

TEST_CASE("Changing a sphere's transform","[sphere]")
{
    Sphere s;
    Matrix t = translation(2,3,4); 
    s.setTransform(t); 
}

TEST_CASE("Intersecting a scaled sphere with a ray","[sphere]")
{
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    Sphere s; 
    s.setTransform(scaling(2,2,2)); 
    std::vector<Intersection> xs = s.intersect(r); 

    REQUIRE(xs.size() == 2); 
    REQUIRE(xs[0].t == 3); 
    REQUIRE(xs[1].t == 7); 
}

TEST_CASE("Intersecting a translated sphere with a ray","[sphere]")
{
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    Sphere s; 
    s.setTransform(translation(5,0,0)); 
    std::vector<Intersection> xs = s.intersect(r); 

    REQUIRE(xs.size() == 0); 
}

TEST_CASE("Computing sphere normal vectors","[sphere]")
{
    Sphere s; 

    SECTION("Normal at point on x-axis")
    {
        Vector n = s.normal_at(Point(1.,0.,0.)); 
        REQUIRE(n == Vector(1.,0.,0.)); 
    }
    SECTION("Normal at point on y-axis")
    {
        Vector n = s.normal_at(Point(0.,1.,0.)); 
        REQUIRE(n == Vector(0.,1.,0.)); 
    }
    SECTION("Normal at point on z-axis")
    {
        Vector n = s.normal_at(Point(0.,0.,1.)); 
        REQUIRE(n == Vector(0.,0.,1.)); 
    }
    SECTION("Normal at non-axial point")
    {
        Vector n = s.normal_at(Point(sqrt(3)/3.f,sqrt(3)/3.f,sqrt(3)/3.f)); 
        REQUIRE(n == Vector(sqrt(3)/3.f,sqrt(3)/3.f,sqrt(3)/3.f)); 
    }
    SECTION("Normal is a normalized vector")
    {
        Vector n = s.normal_at(Point(sqrt(3)/3.f,sqrt(3)/3.f,sqrt(3)/3.f)); 
        REQUIRE(equal_double(n.magnitude(),1.f)); 
    }
}

TEST_CASE("Normal on transformed sphere","[sphere]")
{
    Sphere s;
    SECTION("Normal on translated sphere")
    {
        s.setTransform(translation(0.,1.,0.)); 
        Vector n = s.normal_at(Point(0.f,1.70711,-0.70711)); 
        REQUIRE(n == Vector(0.f,0.70711,-0.70711)); 
    }
    SECTION("Normal on transformed sphere")
    {
        Matrix m = scaling(1,0.5,1) * rotation_z(M_PI/5.f); 
        s.setTransform(m); 
        Vector n = s.normal_at(Point(0.f,sqrt(2)/2.f,-sqrt(2)/2.f)); 
        REQUIRE(n == Vector(0.f,0.97014,-0.24254)); 
    }
}

TEST_CASE("Vector reflection about the normal","[sphere]")
{
    SECTION("reflection of vector approaching at 45 deg")
    {
        Vector v(1.,-1,0); 
        Vector n(0,1,0); 
        Vector r = v.reflect_vector(n); 

        REQUIRE(r == Vector(1,1,0)); 
    }

    SECTION("Reflection off slanted surface")
    {
        Vector v(0.,-1,0); 
        Vector n(sqrt(2)/2.f,sqrt(2)/2.f,0); 

        Vector r = v.reflect_vector(n); 
        REQUIRE(r == Vector(1,0,0)); 
    }
}

TEST_CASE("Sphere material properties","[sphere]")
{
    Sphere s; 
    SECTION("default material")
    {
        Material m = s.getMaterial(); 
        REQUIRE(equal_double(m.ambient,0.1)); 
        REQUIRE(equal_double(m.diffuse,0.9)); 
        REQUIRE(equal_double(m.specular,0.9)); 
        REQUIRE(equal_double(m.shininess,200.0)); 
        REQUIRE(m.mat_color == Color(1,1,1)); 
    }
    SECTION("material assignment")
    {
        Material m(0.1,0.8,0.8,190,Color(1,0,0)); 
        s.setMaterial(m); 

        Material s_m = s.getMaterial(); 
        REQUIRE(equal_double(s_m.ambient,0.1)); 
        REQUIRE(equal_double(s_m.diffuse,0.8)); 
        REQUIRE(equal_double(s_m.specular,0.8)); 
        REQUIRE(equal_double(s_m.shininess,190.0));
        REQUIRE(s_m.mat_color == Color(1,0,0));  
    }
}
