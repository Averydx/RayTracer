#ifndef COLOR_H
#define COLOR_H

#include "tuple.h"
#include <cmath>

class Color : public Tuple
{
/*A tuple representing the RGB values of a color. 
*/

    public: 

        //Constructors
        Color(double r, double g, double b); 

        //Operators
        bool operator==(Color const& obj); 
        Color operator-(Color const& obj); 
        Color operator+(Color const& obj); 
        Color operator*(Color const& obj); 
        Color operator*(double scalar); 

        //Functions
        void clamp(int min, int max); 
}; 

#endif 