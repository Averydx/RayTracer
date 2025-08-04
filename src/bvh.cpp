#include "bvh.h"

#include <utility>
#include <algorithm>
#include <iostream>

// This file defines the Bounding Volume Hierarchy (BVH) for efficient ray tracing
// It includes functions for building the BVH from a list of shapes and performing ray intersection tests

// Helper functions for sorting shapes based on their centroids along different axes
bool comp_xaxis(Shape* a, Shape* b) 
{
    Point c1 = a->transform * Point(0,0,0); 
    Point c2 = b->transform * Point(0,0,0); 
    return c1.x < c2.x;
}

bool comp_yaxis(Shape* a, Shape* b) 
{
    Point c1 = a->transform * Point(0,0,0); 
    Point c2 = b->transform * Point(0,0,0); 
    return c1.y < c2.y;
}

bool comp_zaxis(Shape* a, Shape* b) 
{
    Point c1 = a->transform * Point(0,0,0); 
    Point c2 = b->transform * Point(0,0,0); 
    return c1.z < c2.z;
}


// This function sorts the primitives based on their centroids along the specified axis
// It uses the helper functions defined above to compare the centroids of the shapes
void sortPrimitivesByCentroid(std::vector<Shape*>& primitives, AXIS ax)
{
    switch (ax)
    {
    case AXIS::X_AXIS:
        std::sort(primitives.begin(),primitives.end(),comp_xaxis); 
        return;
    
    case AXIS::Y_AXIS:
        std::sort(primitives.begin(),primitives.end(),comp_yaxis); 
        return;

    case AXIS::Z_AXIS:
        std::sort(primitives.begin(),primitives.end(),comp_zaxis); 
        return;
    }
}

// This function computes the bounding box of a set of primitives and their centroids
// It returns a pair containing the bounding box of the primitives and the bounding box of their centroids
std::pair<AABB,AABB> computeBoundingBox(std::vector<Shape*>& primitives)
{
    AABB primBox = primitives[0]->bounds(); 
    primBox = primBox.transform(primitives[0]->transform); 

    Point centroid = primitives[0]->transform * Point(0,0,0);; 
    AABB cbox = AABB(centroid,centroid); 

    for(Shape* shape: primitives)
    {
       AABB box = shape->bounds(); 
       box = box.transform(shape->transform); 
       primBox = box_union(box,primBox); 

       //centroid calcs 
       Point centroid = shape->transform * Point(0,0,0); 
       cbox.minimum.x = std::min(cbox.minimum.x,centroid.x); 
       cbox.minimum.y = std::min(cbox.minimum.y,centroid.y); 
       cbox.minimum.z = std::min(cbox.minimum.z,centroid.z); 

       cbox.maximum.x = std::max(cbox.maximum.x,centroid.x); 
       cbox.maximum.y = std::max(cbox.maximum.y,centroid.y); 
       cbox.maximum.z = std::max(cbox.maximum.z,centroid.z); 
    }

    std::pair<AABB,AABB> paired = {primBox,cbox}; 

    return paired; 
}

// This function chooses the axis for splitting the bounding box based on the largest extent
// It compares the extents along the x, y, and z axes and returns the axis
AXIS chooseSplitAxis(const AABB& cbox)
{
    Vector extent = cbox.maximum - cbox.minimum; 
    if(extent.x >= extent.y && extent.x >= extent.z)
        return AXIS::X_AXIS;
        
    if(extent.y >= extent.z)
        return AXIS::Y_AXIS; 

    return AXIS::Z_AXIS; 
}

// This function creates a union of two AABBs, returning a new AABB that encompasses both
// It calculates the minimum and maximum points of the union based on the minimum and maximum points of both boxes
AABB box_union(const AABB& box1, const AABB& box2)
{
    Point minUnion; 
    Point maxUnion; 

    minUnion.x = std::min(box1.minimum.x,box2.minimum.x); 
    minUnion.y = std::min(box1.minimum.y,box2.minimum.y); 
    minUnion.z = std::min(box1.minimum.z,box2.minimum.z); 

    maxUnion.x = std::max(box1.maximum.x,box2.maximum.x); 
    maxUnion.y = std::max(box1.maximum.y,box2.maximum.y); 
    maxUnion.z = std::max(box1.maximum.z,box2.maximum.z); 

    return AABB(minUnion,maxUnion); 
}

// This function builds a BVH from a list of shapes, recursively dividing the shapes into left and right subtrees
// It takes a maximum number of primitives per leaf node as a parameter
BVHNode* build_bvh(std::vector<Shape*>& primitives,int maxPrimsPerLeaf)
{
    if(primitives.size() == 0)
    {
        BVHNode* bvh = new BVHNode(); 
        bvh->isLeaf = true; 
        return bvh; 
    }

    return build_bvh_recursive(primitives,maxPrimsPerLeaf); 
}


BVHNode* build_bvh_recursive(std::vector<Shape*>& primitives,int maxPrimsPerLeaf)
{
    //base case
    if(primitives.size() <= maxPrimsPerLeaf)
    {
        BVHNode* node = new BVHNode; 
        auto [bbox, _] = computeBoundingBox(primitives); 
        node->bbox = bbox; 
        node->primitives = primitives; 
        node->isLeaf = true; 

        return node; 
    }

    //Compute the bounding box of all the primitives
    auto [bbox,cbox] = computeBoundingBox(primitives); 
    
    //Choose the split axis based on how far apart the centroids are
    AXIS ax = chooseSplitAxis(cbox); 

    //Sort the primitives based on their centroids along the chosen axis
    sortPrimitivesByCentroid(primitives,ax); 

    //Split the primitives into two halves
    //This is a simple split, could be improved with a more sophisticated method
    int midpoint = primitives.size()/2; 
    std::vector<Shape*> left_primitives = std::vector<Shape*>(primitives.begin(), primitives.begin() + midpoint);
    std::vector<Shape*> right_primitives = std::vector<Shape*>(primitives.begin() + midpoint, primitives.end());

    //Recursively build the left and right subtrees
    BVHNode* node = new BVHNode; 
    node->bbox = bbox; 
    node->left = build_bvh(left_primitives,maxPrimsPerLeaf); 
    node->right = build_bvh(right_primitives,maxPrimsPerLeaf); 

    return node; 
}

// This function counts the number of primitives in the BVH
// It recursively traverses the BVH and sums the number of primitives in each leaf node
int count_bvh(BVHNode* node)
{
    if(node->isLeaf)
        return node->primitives.size(); 

    return count_bvh(node->left) + count_bvh(node->right); 
}

// This function intersects a ray with the BVH
std::vector<Intersection> bvh_intersect(BVHNode* node,const Ray& r)
{
    std::vector<Intersection> xs; 
    bvh_intersect_recursive(node, r, xs); 
    return xs; 
}

bool bvh_intersect_recursive(BVHNode* node, const Ray& r,std::vector<Intersection>& xs)
{
    if(!node->bbox.check_intersect(r))
        return false; 

    if(node->isLeaf)
    {
        bool hitSomething = false; 
        for(Shape* s: node->primitives)
        {
            std::vector<Intersection> temp = s->intersect(r); 
            if(!temp.empty())
            {
                hitSomething = true; 
                xs.insert(xs.end(),temp.begin(),temp.end()); 
            }
        }
        return hitSomething; 
    }

    bool leftHit = bvh_intersect_recursive(node->left,r,xs); 
    bool rightHit = bvh_intersect_recursive(node->right,r,xs); 

    return leftHit || rightHit; 
}

void delete_bvh(BVHNode* node)
{
    if(node == nullptr)
        return; 

    delete_bvh(node->left); 
    delete_bvh(node->right); 

    //std::cout << "Deleting node: " << node << "\n";
    delete node; 
    node=nullptr; 
}

void print_bvh_stats(BVHNode* node, int depth)
{
    {
    if (!node) return;

    std::string indent(depth * 2, ' ');
    std::cout << indent << (node->isLeaf ? "Leaf" : "Internal")
              << " | Address: "<< node
              << " | Depth: " << depth
              << " | Primitives: " << (node->isLeaf ? node->primitives.size() : 0)
              << std::endl;

    if (!node->isLeaf) {
        print_bvh_stats(node->left, depth + 1);
        print_bvh_stats(node->right, depth + 1);
    }
}
}

//Needs thought
void flatten(const std::vector<Shape*>& in_list,std::vector<Shape*>& out_list)
{
    for(Shape* s: in_list)
    {
        if(s->isGroup)
        {
            Group* _s = static_cast<Group*>(s); 
            flatten(_s->children,out_list); 
        }
        out_list.push_back(s);   
    }
}

