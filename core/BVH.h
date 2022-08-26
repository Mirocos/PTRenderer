//
// Created by zeming on 2022/8/13.
//

#ifndef PTRENDERER_BVH_H
#define PTRENDERER_BVH_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "Primitives.h"

struct BVHBuildNode;
using namespace PTRenderer;
class BVHAccel {
public:
    enum class SplitMethod { NAIVE, SAH };

    BVHAccel(const std::vector<std::shared_ptr<Primitives>>& p, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);


    // build method recursively
    std::shared_ptr<BVHBuildNode> recursiveBuild(std::vector<std::shared_ptr<Primitives>> objects);


    // intersection

    Intersection Intersect(const Ray& ray) const;
    Intersection getIntersection(const std::shared_ptr<BVHBuildNode>& node, const Ray& ray) const;
    bool intersectBound(const Ray& ray) const;



public:

    std::shared_ptr<BVHBuildNode> root;

    const int maxPrimsInNode;
    const SplitMethod splitMethod;
    std::vector<std::shared_ptr<Primitives>> primitives;

};



struct BVHBuildNode{
    Bound3 bounds;
    std::shared_ptr<BVHBuildNode> left;
    std::shared_ptr<BVHBuildNode> right;
    std::shared_ptr<Primitives> object;
    float area;

public:
    int splitAxis = 0, firstPrimOffset = 0, nPrimitives = 0;
    BVHBuildNode(){
        bounds = Bound3();
        left = nullptr;
        right = nullptr;
        object = nullptr;
    }

};





#endif //PTRENDERER_BVH_H
