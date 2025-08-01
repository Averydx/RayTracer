#include "bvh.h"

#include <utility>
#include <algorithm>
#include <iostream>

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

AXIS chooseSplitAxis(const AABB& cbox)
{
    Vector extent = cbox.maximum - cbox.minimum; 
    if(extent.x >= extent.y && extent.x >= extent.z)
        return AXIS::X_AXIS;
        
    if(extent.y >= extent.z)
        return AXIS::Y_AXIS; 

    return AXIS::Z_AXIS; 
}


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

BVHNode* build_bvh(std::vector<Shape*>& primitives,int maxPrimsPerLeaf)
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

    sortPrimitivesByCentroid(primitives,ax); 

    int midpoint = primitives.size()/2; 
    std::vector<Shape*> left_primitives = std::vector<Shape*>(primitives.begin(), primitives.begin() + midpoint);
    std::vector<Shape*> right_primitives = std::vector<Shape*>(primitives.begin() + midpoint, primitives.end());

    BVHNode* node = new BVHNode; 
    node->bbox = bbox; 
    node->left = build_bvh(left_primitives,maxPrimsPerLeaf); 
    node->right = build_bvh(right_primitives,maxPrimsPerLeaf); 

    return node; 
}

int count_bvh(BVHNode* node)
{
    if(node->isLeaf)
        return node->primitives.size(); 

    return count_bvh(node->left) + count_bvh(node->right); 
}

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
    if(node->isLeaf)
    {
        delete node; 
        return; 
    }

    delete_bvh(node->left); 
    delete_bvh(node->right); 

    delete node; 
}

void print_bvh_stats(BVHNode* node, int depth)
{
    {
    if (!node) return;

    std::string indent(depth * 2, ' ');
    std::cout << indent << (node->isLeaf ? "Leaf" : "Internal")
              << " | Depth: " << depth
              << " | Primitives: " << (node->isLeaf ? node->primitives.size() : 0)
              << std::endl;

    if (!node->isLeaf) {
        print_bvh_stats(node->left, depth + 1);
        print_bvh_stats(node->right, depth + 1);
    }
}
}

