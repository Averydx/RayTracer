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

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>

TEST_CASE("Creating a ray","[ray]")
{
    Point origin(1,2,3); 
    Vector direction(4,5,6); 

    Ray r(origin,direction); 

    REQUIRE(r.origin == origin); 
    REQUIRE(r.direction == direction); 
}

TEST_CASE("Ray traversal","[ray]")
{
    Ray r(Point(2,3,4),Vector(1,0,0)); 

    Point pos0 = r.position(0); 
    REQUIRE(pos0 == Point(2,3,4)); 

    Point pos1 = r.position(1); 
    REQUIRE(pos1 == Point(3,3,4)); 

    Point pos2 = r.position(-1); 
    REQUIRE(pos2 == Point(1,3,4)); 

    Point pos3 = r.position(2.5); 
    REQUIRE(pos3 == Point(4.5,3,4)); 
}

TEST_CASE("Creating a sphere","[sphere]")
{
    Sphere s(Point(0.f,0.f,0.f),10.f);
    REQUIRE(s.center == Point(0.,0.,0.)); 
    REQUIRE(s.radius == 10.f); 
}

TEST_CASE("Ray sphere intersection")
{
    SECTION("Two intersections")
    {
        Ray r(Point(0,0,-5), Vector(0,0,1)); 
        Sphere s(Point(0.,0.,0.),1.); 

        std::vector<Intersection*> intersections = s.intersect(r); 

        REQUIRE(intersections[0]->t == 4.f); 
        REQUIRE(intersections[1]->t == 6.f); 
    }
    SECTION("One intersection")
    {
        Ray r(Point(0,1,-5), Vector(0,0,1)); 
        Sphere s(Point(0.,0.,0.),1.); 

        std::vector<Intersection*> intersections = s.intersect(r); 

        REQUIRE(intersections[0]->t == 5.f); 
        REQUIRE(intersections[1]->t == 5.f); 
    }

    SECTION("No intersections")
    {
        Ray r(Point(0,2,-5), Vector(0,0,1)); 
        Sphere s(Point(0.,0.,0.),1.); 

        std::vector<Intersection*> intersections = s.intersect(r); 

        REQUIRE(intersections.size() == 0); 
    }

    SECTION("Ray inside sphere")
    {
        Ray r(Point(0,0.,0.), Vector(0,0,1)); 
        Sphere s(Point(0.,0.,0.),1.); 

        std::vector<Intersection*> intersections = s.intersect(r); 

        REQUIRE(intersections.size() == 2); 
        REQUIRE(intersections[0]->t == -1.f); 
        REQUIRE(intersections[1]->t == 1.f); 
    }

    SECTION("Ray in front of sphere")
    {
        Ray r(Point(0,0.,5.), Vector(0,0,1)); 
        Sphere s(Point(0.,0.,0.),1.); 

        std::vector<Intersection*> intersections = s.intersect(r); 

        REQUIRE(intersections.size() == 2); 
        REQUIRE(intersections[0]->t == -6.f); 
        REQUIRE(intersections[1]->t == -4.f); 
    }
}

TEST_CASE("creating intersection","[ray]")
{
    Sphere s(Point(0.f,0.f,0.f),1.f); 
    Intersection i(3.5,&s); 
    REQUIRE(i.t == 3.5); 
    REQUIRE(i.s == &s); 
}

TEST_CASE("Aggregating intersections","ray")
{
    Sphere s(Point(0.f,0.f,0.f),1.f);
    Intersection i1(1.f,&s);
    Intersection i2(2.f,&s);

    std::vector<Intersection*> xs = intersections({&i1,&i2}); 
    REQUIRE(xs.size() == 2); 
    REQUIRE(xs[0]->s == &s); 
    REQUIRE(xs[1]->s == &s); 

}

