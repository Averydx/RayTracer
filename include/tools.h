#ifndef TOOLS_H
#define TOOLS_H

#include <string>

/*
* This file defines tools that may be useful for general computation. 
*
*/

constexpr double EPSILON = 0.01; // A small value used for floating-point comparisons to avoid precision issues
constexpr double BUMP_EPSILON = 1e-3; // A small value used for bumping normal vectors above or below the surface

bool equal_double(double a, double b);  // Compares two double values for equality within a small epsilon range

// Enum for axis representation
// This enum is used to specify the axis along which operations like sorting or checking intersections are performed
enum class AXIS
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS
}; 

#endif