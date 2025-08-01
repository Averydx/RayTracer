#include "pattern.h"
#include "shape.h"

#include <algorithm> 
#include <iostream> 

Color Pattern::color_at_object(const Shape* object, const Point& world_point) const
{
    Point object_point = world_to_object(object,world_point); 
    Point pattern_point = this->transform.inverse() * object_point; 

    return this->color_at(pattern_point); 
} 

Color StripePattern::color_at(const Point& p) const
{
    if((int)floor(p.x) % 2 == 0)
    {
        return this->ca; 
    }
    return this->cb; 
}

Color GradientPattern::color_at(const Point& p) const
{
    double fraction = (p.x + 1.0) / 2.0;
    fraction = std::clamp(fraction, 0.0, 1.0);
    return ca + (cb - ca) * fraction;
}

Color RingPattern::color_at(const Point& p) const
{
    if((int)floor(sqrt(p.x * p.x + p.z * p.z)) % 2 == 0)
        return this->ca; 
    
    return this->cb; 
}

Color CheckerPattern::color_at(const Point& p) const
{
    if((int)(floor(p.x) + floor(p.y) + floor(p.z)) % 2 == 0)
        return this->ca; 
    
    return this->cb; 
}

Color TestPattern::color_at(const Point& p) const 
{
    return Color(p.x,p.y,p.z); 
}