#ifndef BVH_H
#define BVH_H

#include "shape.h"
#include "shapes.h"
#include "tools.h"
#include "intersection.h"

#include <vector>

struct BVHNode
{
    AABB bbox; 
    BVHNode* left = nullptr; 
    BVHNode* right = nullptr; 
    std::vector<Shape*> primitives;  
    bool isLeaf = false; 

}; 

bool comp_xaxis(Shape* a, Shape* b);

bool comp_yaxis(Shape* a, Shape* b); 

bool comp_zaxis(Shape* a, Shape* b);

void sortPrimitivesByCentroid(std::vector<Shape*>& primitives, AXIS ax);

std::pair<AABB,AABB> computeBoundingBox(std::vector<Shape*>& primitives);

AXIS chooseSplitAxis(const AABB& cbox);

AABB box_union(const AABB& box1, const AABB& box2); 

BVHNode* build_bvh(std::vector<Shape*>& primitives, int maxPrimsPerLeaf); 
BVHNode* build_bvh_recursive(std::vector<Shape*>& primitives,int maxPrimsPerLeaf); 

std::vector<Intersection> bvh_intersect(BVHNode* node,const Ray& r); 
bool bvh_intersect_recursive(BVHNode* node, const Ray& r,std::vector<Intersection>& xs); 

int count_bvh(BVHNode* bvh); 

void delete_bvh(BVHNode* node); 

void print_bvh_stats(BVHNode* node, int depth = 0); 

void flatten(const std::vector<Shape*>& in_list,std::vector<Shape*>& out_list);

#endif