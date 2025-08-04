#ifndef BVH_H
#define BVH_H

#include "shape.h"
#include "shapes.h"
#include "tools.h"
#include "intersection.h"

#include <vector>

// This file defines the Bounding Volume Hierarchy (BVH) structure for efficient ray tracing
// It includes the BVHNode class, functions for building the BVH, and intersection methods
struct BVHNode
{
    AABB bbox; 
    BVHNode* left = nullptr; 
    BVHNode* right = nullptr; 
    std::vector<Shape*> primitives;  
    bool isLeaf = false; 

}; 

// Function prototypes for BVH operations
bool comp_xaxis(Shape* a, Shape* b);

bool comp_yaxis(Shape* a, Shape* b); 

bool comp_zaxis(Shape* a, Shape* b);

// Function to sort primitives by their centroid along a specified axis
void sortPrimitivesByCentroid(std::vector<Shape*>& primitives, AXIS ax);

// Function to compute the bounding box of a list of shapes
// Returns a pair of AABBs: the bounding box of the entire list and the bounding box of the centroids
std::pair<AABB,AABB> computeBoundingBox(std::vector<Shape*>& primitives);

// Function to choose the axis for splitting the bounding box based on the longest dimension
// Returns the axis that has the largest extent in the bounding box
AXIS chooseSplitAxis(const AABB& cbox);

// Union of two AABBs to create a new bounding box that contains both
AABB box_union(const AABB& box1, const AABB& box2); 

// Function to build the BVH from a list of shapes
// Takes a vector of shapes and a maximum number of primitives per leaf node
BVHNode* build_bvh(std::vector<Shape*>& primitives, int maxPrimsPerLeaf); 
BVHNode* build_bvh_recursive(std::vector<Shape*>& primitives,int maxPrimsPerLeaf); 

// Function to intersect a ray with the BVH
// Returns a vector of intersections found along the ray
std::vector<Intersection> bvh_intersect(BVHNode* node,const Ray& r); 
bool bvh_intersect_recursive(BVHNode* node, const Ray& r,std::vector<Intersection>& xs); 

// Function to count the number of primitives in the BVH
int count_bvh(BVHNode* bvh); 

// Function to delete the BVH and free memory
void delete_bvh(BVHNode* node); 

// Function to print statistics about the BVH structure
void print_bvh_stats(BVHNode* node, int depth = 0); 

// Function to flatten a BVH into a list of shapes
void flatten(const std::vector<Shape*>& in_list,std::vector<Shape*>& out_list);

#endif