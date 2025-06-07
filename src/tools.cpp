#include <cmath>


const double epsilon = 0.0000001;

bool equal_double(double a, double b)
{
    if(abs(a-b) < epsilon)
    {
        return true; 
    }
    
    return false; 
}

