#include "tools.h"

#include <math.h>

bool equal_double(double a, double b)
{
    if(abs(a-b) < EPSILON)
    {
        return true; 
    }
    
    return false; 
}

