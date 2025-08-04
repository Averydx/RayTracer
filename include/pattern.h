#ifndef PATTERN_H
#define PATTERN_H

#include "color.h"
#include "matrix.h"

class Shape; 

// This file defines the Pattern class and its derived classes for creating various patterns
// Patterns can be applied to shapes to create complex visual effects
class Pattern
{
    public: 
    Pattern(const Color& _ca,const Color& _cb):ca(_ca),cb(_cb),transform(Matrix(4,4)){this->transform.setIdentity();}
    virtual ~Pattern() = default;

    Color ca;
    Color cb; 
    Matrix transform; 
    
    virtual Color color_at(const Point& p) const = 0; 
    Color color_at_object(const Shape* object, const Point& world_point) const; 
}; 

// Derived classes for specific patterns
class StripePattern: public Pattern
{
    public:
    StripePattern(const Color& _ca, const Color& _cb): Pattern(_ca, _cb) {}
    Color color_at(const Point& p) const; 
}; 

class GradientPattern: public Pattern
{
    public:
    GradientPattern(const Color& _ca, const Color& _cb): Pattern(_ca, _cb) {}
    Color color_at(const Point& p) const; 
}; 

class RingPattern: public Pattern
{
    public:
    RingPattern(const Color& _ca, const Color& _cb): Pattern(_ca, _cb) {}
    Color color_at(const Point& p) const; 
}; 

class CheckerPattern: public Pattern
{
    public:
    CheckerPattern(const Color& _ca, const Color& _cb): Pattern(_ca, _cb) {}
    Color color_at(const Point& p) const; 
}; 

class TestPattern: public Pattern
{
    public: 
    TestPattern():Pattern(Color(1,1,1),Color(0,0,0)) {}
    Color color_at(const Point& p) const; 
}; 

#endif