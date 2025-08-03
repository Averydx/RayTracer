#include <catch2/catch_test_macros.hpp>
#include "parser.h"
#include "shapes.h"

TEST_CASE("Vertex loading","[parser]")
{
    Parser p("C:/Users/avery/OneDrive/Desktop/RayTracer/models/vertex_test.obj"); 
    p.read_file(); 

    REQUIRE(p.vertices[0] == Point(-1,1,0)); 
    REQUIRE(p.vertices[1] == Point(-1,0.5,0)); 
    REQUIRE(p.vertices[2] == Point(1,0,0)); 
    REQUIRE(p.vertices[3] == Point(1,1,0)); 
}

TEST_CASE("Parsing triangle faces","[parser]")
{
    Parser p("C:/Users/avery/OneDrive/Desktop/RayTracer/models/faces_test.obj"); 
    p.read_file(); 

    REQUIRE(p.default_group->children.size() == 2); 

    REQUIRE(static_cast<Triangle*>(p.default_group->children[0])->p1 == p.vertices[0]); 
    REQUIRE(static_cast<Triangle*>(p.default_group->children[0])->p2 == p.vertices[1]); 
    REQUIRE(static_cast<Triangle*>(p.default_group->children[0])->p3 == p.vertices[2]); 
    REQUIRE(static_cast<Triangle*>(p.default_group->children[1])->p1 == p.vertices[0]);
    REQUIRE(static_cast<Triangle*>(p.default_group->children[1])->p2 == p.vertices[2]);
    REQUIRE(static_cast<Triangle*>(p.default_group->children[1])->p3 == p.vertices[3]);
}

TEST_CASE("Triangulating polygons")
{
    Parser p("C:/Users/avery/OneDrive/Desktop/RayTracer/models/convex_poly.obj"); 
    p.read_file(); 

    Group* g = p.default_group; 
    Triangle* t1 = static_cast<Triangle*>(g->children[0]); 
    Triangle* t2 = static_cast<Triangle*>(g->children[1]); 
    Triangle* t3 = static_cast<Triangle*>(g->children[2]); 

    REQUIRE(t1->p1 == p.vertices[0]); 
    REQUIRE(t1->p2 == p.vertices[1]); 
    REQUIRE(t1->p3 == p.vertices[2]); 

    REQUIRE(t2->p1 == p.vertices[0]); 
    REQUIRE(t2->p2 == p.vertices[2]); 
    REQUIRE(t2->p3 == p.vertices[3]); 

    REQUIRE(t3->p1 == p.vertices[0]); 
    REQUIRE(t3->p2 == p.vertices[3]); 
    REQUIRE(t3->p3 == p.vertices[4]); 

}
