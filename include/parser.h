#ifndef PARSER_H
#define PARSER_H

#include "point.h"
#include "shape.h"
#include "shapes.h"


#include <iostream> 
#include <string>
#include <vector>


class Parser
{
    public: 
    Parser(const std::string& file_name);
    ~Parser(); 
    void read_file(const char& delimiter = '/'); 

    std::string file_name; 
    Group* default_group; 

    std::vector<Point> vertices = {};
    std::vector<Vector> normals = {}; 

    //helper functions
    std::vector<Triangle*> Parser::fan_triangulation(const std::vector<Point>& vertices) const; 
    std::vector<SmoothTriangle*> Parser::fan_triangulation_smooth(const std::vector<Point>& vertices,const std::vector<Vector>& normals) const; 
}; 
















#endif