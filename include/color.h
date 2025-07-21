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
        Color(); 

        //Operators
        bool operator==(Color const& obj) const; 
        Color operator-(Color const& obj) const; 
        Color operator+(Color const& obj) const; 
        Color operator*(Color const& obj) const; 
        Color operator*(double scalar) const; 

        //Functions
        void clamp(int min, int max); 
}; 

#endif 