#include <catch2/catch_test_macros.hpp>
#include "tuple.h"
#include "point.h"
#include "color.h"
#include "vector.h"
#include "tools.h"
#include "canvas.h"
#include <cmath>
#include <iostream>
#include <fstream>


TEST_CASE( "Point generates points", "[point]" ) {

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

TEST_CASE("Tuple Equality")
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

TEST_CASE("Tuple subtraction", "[subTuple]")
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

TEST_CASE("Negatation of Tuple", "[negTuple]")
{

    SECTION("Vectors")
    {
    Vector v1 =  Vector(1.,1.,1.);

    Vector v3 = -v1;  

    REQUIRE(v3 == Vector(-1.f,-1.f,-1.f)); 
    }

}

TEST_CASE("Scalar multiplication of tuples", "[scaleTuple]")
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

TEST_CASE("Magnitude of Vector","[magVector]")
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

TEST_CASE("Dot product of vectors","[dotVec]")
{
    Vector v1 =  Vector(1.,1.,1.);
    Vector v2 =  Vector(2.,2.,2.); 

    REQUIRE(equal_double(v1 * v2,6.f));  
}

TEST_CASE("Normalization of Vector","[normVec]")
{
    Vector v1 =  Vector(1.,1.,1.); 
    v1.normalize(); 

    REQUIRE(equal_double(v1.magnitude(),1.f)); 
}


TEST_CASE("Cross Product of Vectors","[crossVec]")
{
    Vector v1 = Vector(1.,2.,3.);
    Vector v2 = Vector(2.,3.,4.);

    Vector v3 = v1 ^ v2; 
    Vector v4 = v2 ^ v1; 

    REQUIRE(v3 == Vector(-1.,2.,-1.)); 
    REQUIRE(v4 == Vector(1.,-2., 1.)); 
}

TEST_CASE("Hadamard Product of Colors","[MultColor]")
{
    Color c1 = Color(0.5,0.5,0.5); 
    Color c2 = Color(0.5,0.5,0.5); 
    Color c3 = c1 * c2; 

    REQUIRE(c3 == Color(0.25,0.25,0.25)); 
}

TEST_CASE("Canvas Initialization","[initCanvas]")
{
    int width = 5; 
    int height = 3; 
    Canvas c(width,height); 

    REQUIRE(c.pixel_map.size() == width * height); 

    for(int i = 0; i < c.pixel_map.size(); i++)
    {
        REQUIRE(c.pixel_map.at(i) == Color(0.,0.,0.)); 
    }
}

TEST_CASE("Pixel Getters and Setters","[CanvasPixel]")
{
    int width = 5; 
    int height = 3; 
    Canvas c(width,height); 
    Color red = Color(255,0,0); 

    c.SetPixel(4,2,red); 
    Color c1 = c.GetPixel(4,2); 

    REQUIRE(c1 == red); 

    c.SetPixel(0,0,red); 
    Color c2 = c.GetPixel(0,0); 

    REQUIRE(c2 == red); 

}

TEST_CASE("PixelToPPM","[PixelPPM]")
{
    int width = 5; 
    int height = 3; 
    Canvas c(width,height); 

    Color c1 = Color(1.5,0,0);
    Color c2 = Color(0.,0.5,0.); 
    Color c3 = Color(-0.5,0.,1); 

    c.SetPixel(0,0,c1); 
    c.SetPixel(2,1,c2); 
    c.SetPixel(4,2,c3); 

    c.CanvasToPPM("test1.ppm"); 

    std::ifstream myfile("test1.ppm");
    REQUIRE(myfile.is_open()); 
    std::string str; 
    int index = 0; 
    while(myfile >> str)
    {
        if(index == (4 + 0))
        {
            REQUIRE(str == "255"); 
        }
        if(index == (4 + 1))
        {
            REQUIRE(str == "0"); 
        }
        if(index == (4 + 2))
        {
            REQUIRE(str == "0"); 
        }

        if(index == (25 + 0))
        {
            REQUIRE(str == "0"); 
        }
        if(index == (25 + 1))
        {
            REQUIRE(str == "128"); 
        }
        if(index == (25 + 2))
        {
            REQUIRE(str == "0"); 
        }

        if(index == (46 + 0))
        {
            REQUIRE(str == "0"); 
        }
        if(index == (46 + 1))
        {
            REQUIRE(str == "0"); 
        }
        if(index == (46 + 2))
        {
            REQUIRE(str == "255"); 
        }

        index++; 
    }

    myfile.close(); 

}

TEST_CASE("PPMWrapping","[WrapPPM]")
{
    int width = 10; 
    int height = 2; 
    Canvas c(width,height); 

    c.SetAllPixels(Color(1.,0.8,0.6)); 

    c.CanvasToPPM("test2.ppm"); 

    std::ifstream myfile("test2.ppm");
    REQUIRE(myfile.is_open()); 
    std::string str; 
    int index = 0; 

    while(myfile >> str)
    {
        if(index == 4 + 0)
        {
            REQUIRE(str == "255"); 
        }

        if(index == 4 + 60)
        {
            REQUIRE(str == "153"); 
        }

        index++; 
    }

    myfile.close(); 

}
