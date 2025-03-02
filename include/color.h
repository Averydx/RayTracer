#ifndef COLOR_H
#define COLOR_H

#include "tuple.h"

class Color : public Tuple
{
    public: 

        //Constructors
        Color(double r, double g, double b); 

        //Operators
        bool operator==(Color const& obj); 
        Color operator-(Color const& obj); 
        Color operator+(Color const& obj); 
        Color operator*(Color const& obj); 
        Color operator*(double scalar); 
}; 

#endif 