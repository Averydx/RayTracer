#ifndef TOOLS_H
#define TOOLS_H

#include <string>

/*
* This file defines tools that may be useful for general computation. 
*
*/

constexpr double EPSILON = 0.01; 
constexpr double BUMP_EPSILON = 1e-3; 

bool equal_double(double a, double b); 

enum class AXIS
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS
}; 

#endif