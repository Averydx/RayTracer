#ifndef TUPLE_H
#define TUPLE_H

#include <memory> 

/**
 * Implementation of a Tuple. This class is generally not used directly
 * and serves as a base class for Vectors, Points, and Colors. 
 */
class Tuple
{
    public: 
        double x; 
        double y; 
        double z; 
        double w;

        //Constructors
        Tuple(double x,double y,double z,double w);
        Tuple(); 

};

#endif