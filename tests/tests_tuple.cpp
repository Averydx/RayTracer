#include <catch2/catch_test_macros.hpp>

#include "tuple.h"
#include "point.h"
#include "color.h"
#include "vector.h"
#include "tools.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>

TEST_CASE( "Point generates points", "[point]") {

    Point p1 = Point(1.,2.,3.); 
   

    REQUIRE(equal_double(p1.x,1.f));
    REQUIRE(equal_double(p1.y,2.f));
    REQUIRE(equal_double(p1.z,3.f));
    REQUIRE(equal_double(p1.w,1.f));

}

TEST_CASE("vector generates vectors", "[vector]")
{
    Vector v1 = Vector(1.,2.,3.); 

    REQUIRE(equal_double(v1.x,1.f));
    REQUIRE(equal_double(v1.y,2.f));
    REQUIRE(equal_double(v1.z,3.f));
    REQUIRE(equal_double(v1.w,0.f));

}

TEST_CASE("Tuple Equality","[tuple]")
{
    SECTION("Vector equality")
    {
        Vector v1 =  Vector(1.,2.,3.);
        Vector v2 =  Vector(1.,2.,3.);

        REQUIRE(v1 == v2); 
    }

    SECTION("Point equality")
    {
        Point p1 =  Point(1.,2.,3.);
        Point p2 =  Point(1.,2.,3.);

        REQUIRE(p1 == p2); 
    }

    SECTION("Color equality")
    {
        Color c1 =  Color(1.,2.,3.);
        Color c2 =  Color(1.,2.,3.);

        REQUIRE(c1 == c2); 
    }
}

TEST_CASE("Tuple subtraction", "[tuple]")
{

    SECTION("Vectors")
    {

    Vector v1 =  Vector(1.,1.,1.);
    Vector v2 =  Vector(2.,2.,2.); 

    Vector v3 = v1 - v2;  

    REQUIRE(v3 == Vector(-1.f,-1.f,-1.f));

    }

    SECTION("Points")
    {
        Point p1 = Point(1.,1.,1.);
        Point p2 = Point(2.,2.,2.); 

        Vector v3 = p1 - p2; 

        REQUIRE(v3 == Vector(-1.f,-1.f,-1.f)); 
    }

    SECTION("Colors")
    {
        Color c1 = Color(0.2,0.6,0.6);
        Color c2 = Color(0.2,0.4,0.3); 

        Color c3 = c1 - c2; 

        REQUIRE(c3 == Color(0.0,0.2,0.3)); 
    }

}

TEST_CASE("Vector-Point addition", "[vector]")
{
    Point p1 = Point(1.,1.,1.);
    Vector v1 = Vector(0.5,0.,0.5);
    
    Point p2 = p1 + v1; 
    REQUIRE(p2 == Point(1.5,1.0,1.5)); 

}


TEST_CASE("Negatation of Tuple", "[tuple]")
{

    SECTION("Vectors")
    {
    Vector v1 =  Vector(1.,1.,1.);

    Vector v3 = -v1;  

    REQUIRE(v3 == Vector(-1.f,-1.f,-1.f)); 
    }

}

TEST_CASE("Scalar multiplication of tuples", "[tuple]")
{

    SECTION("Vectors")
    {

    Vector v1 = Vector(1.,1.,1.);

    Vector v3 = -5.f * v1;  

    REQUIRE(v3 == Vector(-5.f,-5.f,-5.f)); 
    }

    SECTION("Scalar Multiplication of Colors")
    {
        Color c1 = Color(0.5,0.5,0.5); 
        c1 = c1 * 0.5; 

        REQUIRE(c1 == Color(0.25,0.25,0.25)); 
    }

}

TEST_CASE("Magnitude of Vector","[vector]")
{
    Vector* t1 = new Vector(1.,0.,0.);

    REQUIRE(equal_double(t1->magnitude(),1.f));

    t1->x = 0.f; 
    t1->y = 1.f; 

    REQUIRE(equal_double(t1->magnitude(),1.f)); 

    t1->y = 0.f; 
    t1->z = 1.f; 

    REQUIRE(equal_double(t1->magnitude(),1.f)); 

    t1->x = 1; 
    t1->y = 2; 
    t1->z = 3; 

    REQUIRE(equal_double(t1->magnitude(),sqrt(14))); 

    Vector t3 = -*t1; 

    REQUIRE(equal_double(t3.magnitude(),sqrt(14))); 
}

TEST_CASE("Dot product of vectors","[vector]")
{
    Vector v1 =  Vector(1.,1.,1.);
    Vector v2 =  Vector(2.,2.,2.); 

    REQUIRE(equal_double(v1 * v2,6.f));  
}

TEST_CASE("Normalization of Vector","[vector]")
{
    Vector v1 =  Vector(1.,1.,1.); 
    v1.normalize(); 

    REQUIRE(equal_double(v1.magnitude(),1.f)); 
}


TEST_CASE("Cross Product of Vectors","[vector]")
{
    Vector v1 = Vector(1.,2.,3.);
    Vector v2 = Vector(2.,3.,4.);

    Vector v3 = v1 ^ v2; 
    Vector v4 = v2 ^ v1; 

    REQUIRE(v3 == Vector(-1.,2.,-1.)); 
    REQUIRE(v4 == Vector(1.,-2., 1.)); 
}

TEST_CASE("Hadamard Product of Colors","[color]")
{
    Color c1 = Color(0.5,0.5,0.5); 
    Color c2 = Color(0.5,0.5,0.5); 
    Color c3 = c1 * c2; 

    REQUIRE(c3 == Color(0.25,0.25,0.25)); 
}
