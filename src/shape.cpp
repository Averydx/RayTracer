#include "shape.h"
#include "materials.h"

Matrix Shape::getTransform() const
{
    return this->transform; 
}

void Shape::setTransform(const Matrix& m)
{
    this->transform = m; 
}

void Shape::setMaterial(const Material& m)
{
    this->mat = m; 
}

Material Shape::getMaterial() const
{
    return this->mat; 
}
