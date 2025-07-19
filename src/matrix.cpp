#include "matrix.h"
#include "tools.h"

#include <stdexcept>
#include <iostream>

Matrix::Matrix(int rows, int cols)
{
    this->rows = rows; 
    this->cols = cols; 

    this->data = std::vector<double>(); 

    this->data.resize(rows*cols,1); 
}

Matrix::Matrix(int rows, int cols, std::vector<double>& init_data)
{
    this->rows = rows; 
    this->cols = cols; 

    this->data = std::vector<double>(); 

    this->data.resize(rows*cols,1); 

    this->setAllElements(init_data); 
}

double Matrix::getElement(int row,int col) const
{
    return this->data.at(row * cols + col); 
}

void Matrix::printMatrix()
{
    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            std::cout<<this->data[i * cols + j] << " "; 
        }
        std::cout<<std::endl; 
    }
}

void Matrix::setElement(int row,int col, double value)
{
    this->data.at(row * cols + col) = value; 
}

void Matrix::setAllElements(std::vector<double>& data)
{
    if(data.size() != this->data.size())
        throw std::invalid_argument("data array and matrix dimension mismatch!"); 

    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            this->data[i * cols + j] = data[i * cols + j]; 
        }
    }
}

void Matrix::setIdentity()
{
    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            if(i == j)
                this->data[i * cols + j] = 1.f;
            else 
                this->data[i * cols + j] = 0.f;  

        }
    }
}

double Matrix::determinant()
{
    if(rows != cols)
    {
        throw std::invalid_argument("Matrix is non-square!"); 
    }

    double det = 0;

    if(rows ==2 && cols ==2)
        det = this->getElement(0,0) * this->getElement(1,1) - this->getElement(0,1) * this->getElement(1,0); 
    else 
        {
            for(int i = 0; i < this->cols; i++)
            {
                det = det + this->getElement(0,i) * this->cofactor(0,i); 
            }
        }

    return det; 
}

void Matrix::transpose()
{
    int old_rows = this->rows; 
    int old_cols = this->cols; 

    this->rows = old_cols; 
    this->cols = old_rows; 
    
    std::vector<double> new_data;
    new_data.resize(rows*cols,1); 

    for(int i = 0; i < old_rows; i++)
    {
        for(int j = 0; j < old_cols;j++)
        {
            int old_index = i * old_cols + j; 
            int new_index = j * old_rows + i; 

            new_data[new_index] = data[old_index]; 
        }
    }

    for(int i = 0; i < data.size();i++)
    {
        this->data[i] = new_data[i]; 
    }
    
}

Matrix Matrix::subMatrix(int rowToRemove,int colToRemove)
{
    Matrix sub(rows -1, cols -1); 
    int subIndex = 0; 

    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            if(i != rowToRemove && j != colToRemove)
            {
                sub.data[subIndex] = data[i * cols + j]; 
                subIndex++; 
            }
        }
    }

    return sub; 
}

double Matrix::minor(int rowToRemove,int colToRemove)
{
    Matrix subMatrix = this->subMatrix(rowToRemove,colToRemove); 
    return subMatrix.determinant(); 
}

double Matrix::cofactor(int rowToRemove,int colToRemove)
{
    if((rowToRemove + colToRemove) % 2 != 0)
        return -1 * this->minor(rowToRemove,colToRemove); 

    return this->minor(rowToRemove,colToRemove);
}

Matrix Matrix::inverse()
{
    if(this->determinant() == 0)
        throw std::invalid_argument("Matrix has determinant 0, it is not invertible!"); 

    Matrix inv_m = Matrix(this->rows,this->cols); 
    double det = this->determinant(); 

    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            double c = this->cofactor(i,j); 

            inv_m.setElement(j,i,c / det); 
        }
    }

    return inv_m; 
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
            double value = 0; 
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



