#ifndef VECTOR_H
#define VECTOR_H

#include "tuple.h"

/*
 *  This class defines a Vector. The overloaded operators support vector-scalar products and operations, 
 *  vector-vector operations including dot and cross products, 
 *  and normalization magnitude calculations.  
 */
class Vector : public Tuple
{
    public: 
        //Constructors
        Vector(double x, double y, double z); 
        Vector(); 

        //Operators
        Vector operator+(Vector const& obj);
        Vector operator-(Vector const& obj);
        Vector operator-(); 
        Vector operator*(double scalar); 
        double operator*(Vector const& obj); 
        Vector operator^(Vector const& obj); 
        bool operator==(Vector const& obj); 

        //Methods
        double magnitude(); 
        void normalize(); 

        
}; 

Vector operator*(double scalar,Vector const& obj); 

#endif