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


TEST_CASE("Canvas Initialization","[canvas]")
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

TEST_CASE("Pixel Getters and Setters","[canvas]")
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

TEST_CASE("PixelToPPM","[ppm]")
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

TEST_CASE("PPMWrapping","[ppm]")
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