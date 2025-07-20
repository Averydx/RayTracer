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


TEST_CASE("MatrixAccess","[matrix]")
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

TEST_CASE("MatrixEquality","[matrix]")
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

TEST_CASE("MatrixMultSquare","[matrix]")
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

TEST_CASE("MatrixMultNonSquare","[matrix]")
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

TEST_CASE("MatrixTupleMultiplication","[matrix]")
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

TEST_CASE("MatrixIdentityMult","[matrix]")
{
    Matrix m(4,4);
    Matrix identity(4,4); 
    identity.setIdentity(); 
    std::vector<double> data = {1,2,3,4,2,4,4,2,8,6,4,1,0,0,0,1}; 
    m.setAllElements(data); 

    Matrix result = m * identity; 

    REQUIRE(m == result); 
}

TEST_CASE("MatrixTranspose","[matrix]")
{
    Matrix m(3,4); 
    std::vector<double> data = {
        1,2,3,4,
        2,4,4,2,
        8,6,4,1}; 

    m.setAllElements(data); 
    m.transpose(); 

    REQUIRE(m.rows == 4); 
    REQUIRE(m.cols == 3); 

    Matrix desiredResult(4,3); 
    std::vector<double> desiredResultData = {
        1,2,8,
        2,4,6,
        3,4,4,
        4,2,1}; 

    desiredResult.setAllElements(desiredResultData); 
    REQUIRE(m == desiredResult); 
}

TEST_CASE("2x2Determinant","[matrix]")
{
    Matrix m(2,2); 
    std::vector<double> data = {1,5,-3,2};
    m.setAllElements(data); 

    double det = m.determinant();
    
    REQUIRE(det == 17); 
}

TEST_CASE("subMatrices","[matrix]")
{
    Matrix t1(3,3); 
    std::vector<double> data1 = {1,5,0,-3,2,7,0,6,-3};
    t1.setAllElements(data1); 

    Matrix s1 = t1.subMatrix(0,2); 

    std::vector<double> desired_data1 = {-3,2,0,6};

    REQUIRE(s1 == Matrix(2,2,desired_data1)); 

    Matrix t2(4,4); 
    std::vector<double> data2 = {-6,1,1,6,-8,5,8,6,-1,0,8,2,-7,1,-1,1};
    t2.setAllElements(data2); 

    Matrix s2 = t2.subMatrix(2,1); 

    std::vector<double> desired_data2 = {-6,1,6,-8,8,6,-7,-1,1};
    REQUIRE(s2 == Matrix(3,3,desired_data2)); 
    
}

TEST_CASE("matrixMinor","[matrix]")
{
    Matrix A(3,3); 
    std::vector<double> data1 = {3,5,0,2,-1,-7,6,-1,5};
    A.setAllElements(data1); 

    Matrix B = A.subMatrix(1,0); 
    REQUIRE(B.determinant() == 25); 
    REQUIRE(A.minor(1,0) == 25); 
}

TEST_CASE("matrixCofactor","[matrix]")
{
    Matrix A(3,3); 
    std::vector<double> data1 = {3,5,0,2,-1,-7,6,-1,5};
    A.setAllElements(data1); 

    REQUIRE(A.minor(0,0) == -12);
    REQUIRE(A.cofactor(0,0) == -12);

    REQUIRE(A.minor(1,0) == 25);
    REQUIRE(A.cofactor(1,0) == -25);
}

TEST_CASE("fullDeterminant","[matrix]")
{
    Matrix A(3,3); 
    std::vector<double> dataA = {1,2,6,-5,8,-4,2,6,4};
    A.setAllElements(dataA); 

    REQUIRE(A.cofactor(0,0) == 56); 
    REQUIRE(A.cofactor(0,1) == 12); 
    REQUIRE(A.cofactor(0,2) == -46); 
    REQUIRE(A.determinant() == -196); 

    Matrix B(4,4); 
    std::vector<double> dataB = {-2,-8,3,5,-3,1,7,3,1,2,-9,6,-6,7,7,-9};
    B.setAllElements(dataB); 

    REQUIRE(B.cofactor(0,0) == 690); 
    REQUIRE(B.cofactor(0,1) == 447); 
    REQUIRE(B.cofactor(0,2) == 210); 
    REQUIRE(B.cofactor(0,3) == 51); 
    REQUIRE(B.determinant() == -4071); 

}

TEST_CASE("matrixInverse","[matrix]")
{
    Matrix A(4,4); 
    std::vector<double> dataA = {-5,2,6,-8,1,-5,1,8,7,7,-6,-7,1,-3,7,4};
    A.setAllElements(dataA); 

    Matrix B = A.inverse(); 

    REQUIRE(A.determinant() == 532); 
    REQUIRE(A.cofactor(2,3) == -160); 
    REQUIRE(equal_double(B.getElement(3,2),-160.0/532.0)); 

    REQUIRE(A.cofactor(3,2) == 105); 
    REQUIRE(equal_double(B.getElement(2,3),105.0/532.0)); 

    std::vector<double> dataInverse = {
        0.21805,0.45113,0.24060,-0.04511,
        -0.80827,-1.45677,-0.44361,0.52068,
        -0.07895,-0.22368,-0.05263,0.19737,
        -0.52256,-0.81391,-0.30075,0.30639};

    REQUIRE(B == Matrix(4,4,dataInverse)); 

}

TEST_CASE("MatrixInverseProductIsIdentity","[matrix]")
{
    Matrix A(4,4); 
    std::vector<double> dataA = {
        3,-9,7,3,
        3,-8,2,-9,
        -4,4,4,1,
        -6,5,-1,1};
    A.setAllElements(dataA); 

    Matrix I = A.inverse() * A; 
    Matrix identity(4,4); 
    identity.setIdentity(); 

    REQUIRE(I == identity); 
}