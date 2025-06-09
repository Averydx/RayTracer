#include <catch2/catch_test_macros.hpp>

#include "tuple.h"
#include "point.h"
#include "color.h"
#include "vector.h"
#include "tools.h"
#include "canvas.h"
#include "matrix.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <array>


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

TEST_CASE("Vector-Point addition", "[addVecPoint]")
{
    Point p1 = Point(1.,1.,1.);
    Vector v1 = Vector(0.5,0.,0.5);
    
    Point p2 = p1 + v1; 
    REQUIRE(p2 == Point(1.5,1.0,1.5)); 

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

TEST_CASE("MatrixAccess","[MatrixCheck]")
{

    //4x4 matrix
    Matrix m1(4,4); 
    std::array<double,16> data1 = {1,2,3,4,5.5,6.5,7.5,8.5,9,10,11,12,13.5,14.5,15.5,16.5};  

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m1.setElement(i,j,data1[i * 4 + j]); 
            REQUIRE(equal_double(m1.getElement(i,j),data1[i * 4 + j]));  
        }
    }

    //2x2 matrix
    Matrix m2(2,2); 
    std::array<double,4> data2 = {-3,5,1,-2};  

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            m2.setElement(i,j,data2[i * 2 + j]); 
            REQUIRE(equal_double(m2.getElement(i,j),data2[i * 2 + j])); 
        }
    }

    //3x3 matrix
    Matrix m3(3,3); 
    std::array<double,9> data3 = {-3,5,0,1,-2,-7,0,1,1};  

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            m3.setElement(i,j,data3[i * 3 + j]); 
            REQUIRE(equal_double(m3.getElement(i,j),data3[i * 3 + j])); 
        }
    }

    Matrix m4(3,2); 
    std::array<double,6> data4 = {-3,5,0,1,-2,-7};  

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            m4.setElement(i,j,data4[i * 2 + j]); 
            REQUIRE(equal_double(m4.getElement(i,j),data4[i * 2 + j])); 
        }
    }

}

TEST_CASE("MatrixEquality","[MatrixEqual]")
{
    Matrix m1(4,4);
    std::vector<double> data = {1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2};  
    m1.setAllElements(data); 
    
    Matrix m2(4,4); 
    m2.setAllElements(data); 

    REQUIRE(m1 == m2); 

    m1.setElement(0,0,0); 

    REQUIRE(!(m1 == m2)); 
}

TEST_CASE("MatrixMultSquare","[MatrixMultSquare]")
{
    Matrix m1(3,3);
    std::vector<double> data = {1,2,3,4,5,6,7,8,9}; 
    m1.setAllElements(data); 

    Matrix m2(3,3); 
    std::vector<double> data2 = {9,8,7,6,5,4,3,2,1}; 
    m2.setAllElements(data2); 

    Matrix result = m1 * m2; 

    std::vector<double> desiredData = {30,24,18,84,69,54,138,114,90}; 
    Matrix desiredResult(3,3); 
    desiredResult.setAllElements(desiredData); 

    REQUIRE(result == desiredResult); 

}

TEST_CASE("MatrixMultNonSquare","[MatrixMultNonSquare]")
{
    Matrix m1(3,3);
    std::vector<double> data = {1,2,3,4,5,6,7,8,9}; 
    m1.setAllElements(data); 

    Matrix m2(3,2); 
    std::vector<double> data2 = {6,5,4,3,2,1}; 
    m2.setAllElements(data2); 

    Matrix result = m1 * m2; 

    std::vector<double> desiredData = {20,14,56,41,92,68}; 
    Matrix desiredResult(3,2); 
    desiredResult.setAllElements(desiredData); 

    REQUIRE(result == desiredResult); 

}

TEST_CASE("MatrixTupleMultiplication","[MatTupleMult]")
{
    Point p(1,2,3); 
    Matrix m(4,4);
    std::vector<double> data = {1,2,3,4,2,4,4,2,8,6,4,1,0,0,0,1}; 
    m.setAllElements(data); 

    Point result1 = m * p; 

    REQUIRE(result1 == Point(18,24,33)); 

    Vector v(1,2,3);
    Vector result2 = m * v; 

    REQUIRE(result2 == Vector(14,22,32)); 
    
}