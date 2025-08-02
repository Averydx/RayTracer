#include "bvh.h"
#include "shape.h"
#include "shapes.h"
#include "transformations.h"

#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Creating and deleting bvh","[bvh]")
{
    std::vector<Shape*> list; 

    for(int i = 0; i < 10; i++)
    {
        Sphere* s = new Sphere(); 
        s->transform = translation(0,i,0); 
        list.push_back(s); 
    }

    BVHNode* bvh = build_bvh(list,2); 

    REQUIRE(bvh != nullptr); 
    REQUIRE(count_bvh(bvh) == 10); 

    delete_bvh(bvh); 

    for(Shape* s: list)
    {
        delete s; 
    }
}

TEST_CASE("Bounding box for a triangle","[triangle][shapes][bvh]")
{
    Point p1(-3,7,2); 
    Point p2(6,2,-4); 
    Point p3(2,-1,-1); 

    Triangle t(p1,p2,p3); 
    AABB box = t.bounds(); 

    REQUIRE(box.minimum == Point(-3,-1,-4)); 
    REQUIRE(box.maximum == Point(6,7,2));
}

