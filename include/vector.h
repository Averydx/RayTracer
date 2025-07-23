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
        Vector operator+(Vector const& obj) const;
        Vector operator-(Vector const& obj) const;
        Vector operator-() const; 
        Vector operator*(double scalar) const; 
        double operator*(Vector const& obj) const; 
        Vector operator^(Vector const& obj) const; 
        bool operator==(Vector const& obj) const; 

        //Methods
        double magnitude() const; 
        Vector normalize() const; 
        void printVector() const; 
        Vector reflect_vector(const Vector& normal) const; 

        
}; 

Vector operator*(double scalar,Vector const& obj); 

#endif