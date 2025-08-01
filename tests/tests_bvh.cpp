#include "bvh.h"
#include "shape.h"
#include "shapes.h"
#include "transformations.h"

#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Creating a bvh","[bvh]")
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
}