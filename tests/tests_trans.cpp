#include <catch2/catch_test_macros.hpp>

#include "tuple.h"
#include "point.h"
#include "color.h"
#include "vector.h"
#include "tools.h"
#include "canvas.h"
#include "matrix.h"
#include "transformations.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>


TEST_CASE("Translation","[transformations]")
{
    Matrix A = translation(5,-3,2); 
    Point p1(-3,4,5); 
    Point p2 = A * p1; 

    REQUIRE(p2 == Point(2,1,7)); 

    Matrix inv_A = A.inverse(); 
    Point p3 = Point(-3,4,5); 
    Point p4 = inv_A * p3; 

    REQUIRE(p4 == Point(-8,7,3));
    
    Vector v1(-3,4,5); 
    Vector v2 = A * v1; 

    REQUIRE(v1 == v2); 
}

TEST_CASE("Scaling","[transformations]")
{

    Matrix A = scaling(2,3,4); 
    SECTION("Points")
    {

        Point p1(-4,6,8); 
        Point p2 = A * p1; 
        REQUIRE(p2 == Point(-8,18,32)); 

    }

    SECTION("Vectors")
    {
        Vector v1(-4,6,8); 
        Vector v2 = A * v1; 
        REQUIRE(v2 == Vector(-8,18,32)); 
    }
    
}

TEST_CASE("Reflection","[transformations]")
{
    Matrix A = scaling(-1,1,1); 
    Point p1(2,3,4); 
    Point p2 = A * p1; 

    REQUIRE(p2 == Point(-2,3,4)); 
}

TEST_CASE("Rotation","[transformations]")
{
    SECTION("X Rotation")
    {
        Matrix A = rotation_x(M_PI/4.f); 
        Point p1(0,1,0); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(0,sqrt(2)/2,sqrt(2)/2)); 

        Matrix B = rotation_x(M_PI/2.f); 
        p1 = Point(0,1,0); 
        p2 = B * p1; 

        REQUIRE(p2 == Point(0,0,1)); 
    }

    SECTION("Y Rotation")
    {
        Matrix A = rotation_y(M_PI/4.f); 
        Point p1(0,0,1); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(sqrt(2)/2,0,sqrt(2)/2)); 

        Matrix B = rotation_y(M_PI/2.f); 
        p1 = Point(0,0,1); 
        p2 = B * p1; 

        REQUIRE(p2 == Point(1,0,0)); 
    }

    SECTION("Z Rotation")
    {
        Matrix A = rotation_z(M_PI/4.f); 
        Point p1(0,1,0); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(-sqrt(2)/2,sqrt(2)/2,0)); 

        Matrix B = rotation_z(M_PI/2.f); 
        p1 = Point(0,1,0); 
        p2 = B * p1; 

        REQUIRE(p2 == Point(-1,0,0)); 
    }

}

TEST_CASE("Skew","[transformations]")
{
    SECTION("Test x_y")
    {
        Matrix A = skew(1,0,0,0,0,0); 
        Point p1 = Point(2,3,4); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(5,3,4)); 
    }

    SECTION("Test x_z")
    {
        Matrix A = skew(0,1,0,0,0,0); 
        Point p1 = Point(2,3,4); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(6,3,4)); 
    }

    SECTION("Test y_x")
    {
        Matrix A = skew(0,0,1,0,0,0); 
        Point p1 = Point(2,3,4); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(2,5,4)); 
    }

    SECTION("Test y_z")
    {
        Matrix A = skew(0,0,0,1,0,0); 
        Point p1 = Point(2,3,4); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(2,7,4)); 
    }

    SECTION("Test z_x")
    {
        Matrix A = skew(0,0,0,0,1,0); 
        Point p1 = Point(2,3,4); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(2,3,6)); 
    }

    SECTION("Test z_x")
    {
        Matrix A = skew(0,0,0,0,0,1); 
        Point p1 = Point(2,3,4); 
        Point p2 = A * p1; 

        REQUIRE(p2 == Point(2,3,7)); 
    }
}

TEST_CASE("Chained Transformations")
{
    SECTION("Apply one by one")
    {
        Point p1(1,0,1); 
        Matrix A = rotation_x(M_PI/2.f); 
        Matrix B = scaling(5,5,5); 
        Matrix C = translation(10,5,7); 

        //apply rotation first 
        Point p2 = A * p1; 
        REQUIRE(p2 == Point(1,-1,0)); 
        //then apply scaling
        Point p3 = B * p2; 
        REQUIRE(p3 == Point(5,-5,0)); 
        //then apply translation 
        Point p4 = C * p3; 
        REQUIRE(p4 == Point(15,0,7)); 
    }
    SECTION("Chained transformations")
    {
        Point p1(1,0,1); 
        Matrix A = rotation_x(M_PI/2.f); 
        Matrix B = scaling(5,5,5); 
        Matrix C = translation(10,5,7); 

        Matrix T = C * B * A; 
        Point p2 = T * p1; 

        REQUIRE(p2 == Point(15,0,7)); 
    }

}