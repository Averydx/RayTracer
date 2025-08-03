#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

Parser::Parser(const std::string& file_name)
{
    this->file_name = file_name; 
    std::ifstream file(file_name); 
    if(!file.is_open())
    {
        throw std::exception("Model file could not be opened. "); 
    }

    this->default_group = new Group(); 
}

Parser::~Parser()
{
    
}

void Parser::read_file()
{
    std::stringstream ss; 
    std::ifstream file(this->file_name); 

    std::string line = ""; 
    std::string prefix = ""; 

    Point vertex; 

    while(std::getline(file,line))
    {
        //Get the prefix of the line
        ss.clear(); 
        ss.str(line); 
        ss >> prefix; 

        if(prefix == "v")
        {
            ss >> vertex.x >> vertex.y >> vertex.z; 
            vertices.push_back(vertex); 
        }

        if(prefix == "f")
        {
            int counter = 0;  
            std::vector<Point> poly_vertices; 
            std::string vertex_string = "";
            while(ss >> vertex_string)
            {   
                std::stringstream vss(vertex_string);
                std::string v_idx_str, vt_idx_str, vn_idx_str;

                std::getline(vss, v_idx_str, '/');      // vertex index
                std::getline(vss, vt_idx_str, '/');     // texture index (may be empty)
                std::getline(vss, vn_idx_str, '/');     // normal index (may be empty)

                int v_idx = std::stoi(v_idx_str);
                poly_vertices.push_back(vertices[v_idx - 1]); // 1-based indexing
            }
            
            std::vector<Triangle*> triangles = this->fan_triangulation(poly_vertices); 
            for(Triangle* tri: triangles)
            {
                default_group->add_child(tri); 
            }
            
        }
    }

}

    std::vector<Triangle*> Parser::fan_triangulation(const std::vector<Point>& vertices) const
    {
        std::vector<Triangle*> triangles; 

        for(int i = 1;  i < vertices.size()-1; i++)
        {
            Triangle* tri = new Triangle(vertices[0],vertices[i],vertices[i+1]); 
            triangles.push_back(tri); 
        }

        return triangles; 
    }