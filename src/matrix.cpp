#include "matrix.h"
#include "tools.h"

#include <stdexcept>

Matrix::Matrix(int rows, int cols)
{
    this->rows = rows; 
    this->cols = cols; 

    this->data = std::vector<double>(); 

    this->data.resize(rows*cols,1); 
}

double Matrix::getElement(int row,int col) const
{
    return this->data.at(row * cols + col); 
}

void Matrix::setElement(int row,int col, double value)
{
    this->data.at(row * cols + col) = value; 
}

void Matrix::setAllElements(std::vector<double>& data)
{
    if(data.size() != this->data.size())
        return; 

    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            this->data[i * cols + j] = data[i * cols + j]; 
        }
    }
}
   

bool Matrix::operator==(Matrix const& obj)
{
    if((this->rows != obj.rows) || (this->rows != obj.rows))
        return false; 

    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->cols;j++)
        {
            if(!equal_double(this->getElement(i,j),obj.getElement(i,j)))
            {
                return false; 
            }
        }
    }

    return true; 
}

Matrix Matrix::operator*(Matrix const& obj)
{
    if(this->cols != obj.rows)
    {
        throw std::invalid_argument("Matrix dimensions are incompatible!"); 
    }

    Matrix result(this->rows,obj.cols); 
    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < obj.cols;j++)
        {
            int value = 0; 
            for(int k = 0; k < this->cols;k++)
            {
                value += this->getElement(i,k) * obj.getElement(k,j); 
            }

            result.setElement(i,j,value); 
        }
    }

    return result; 
}

Vector Matrix::operator*(Vector const& obj)
{
    if(this->cols != 4)
    {
        throw std::invalid_argument("Matrix dimensions are incompatible for tuple multiplication!"); 
    }

    Vector result; 
    result.x = this->getElement(0,0) * obj.x + this->getElement(0,1) * obj.y + this->getElement(0,2) * obj.z + this->getElement(0,3) * obj.w;
    result.y = this->getElement(1,0) * obj.x + this->getElement(1,1) * obj.y + this->getElement(1,2) * obj.z + this->getElement(1,3) * obj.w;
    result.z = this->getElement(2,0) * obj.x + this->getElement(2,1) * obj.y + this->getElement(2,2) * obj.z + this->getElement(2,3) * obj.w;
    result.w = this->getElement(3,0) * obj.x + this->getElement(3,1) * obj.y + this->getElement(3,2) * obj.z + this->getElement(3,3) * obj.w;

    return result; 
}

Point Matrix::operator*(Point const& obj)
{
    if(this->cols != 4)
    {
        throw std::invalid_argument("Matrix dimensions are incompatible for tuple multiplication!"); 
    }

    Point result; 
    result.x = this->getElement(0,0) * obj.x + this->getElement(0,1) * obj.y + this->getElement(0,2) * obj.z + this->getElement(0,3) * obj.w;
    result.y = this->getElement(1,0) * obj.x + this->getElement(1,1) * obj.y + this->getElement(1,2) * obj.z + this->getElement(1,3) * obj.w;
    result.z = this->getElement(2,0) * obj.x + this->getElement(2,1) * obj.y + this->getElement(2,2) * obj.z + this->getElement(2,3) * obj.w;
    result.w = this->getElement(3,0) * obj.x + this->getElement(3,1) * obj.y + this->getElement(3,2) * obj.z + this->getElement(3,3) * obj.w;

    return result; 
}

