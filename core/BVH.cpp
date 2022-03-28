//
// Created by zeming on 2022/3/15.
//

#include <algorithm>
#include "BVH.h"

struct  BVHPrimitiveInfo {
    BVHPrimitiveInfo(){}
    BVHPrimitiveInfo(size_t _pN, const BoundingBox& bbx)
    : primitiveNumber(_pN), bounds(bbx), centroid(.5f * bounds.pmin + .5f * bounds.pmax){

    }


    size_t primitiveNumber;
    BoundingBox bounds;
    glm::vec3 centroid;
};


struct BVHBuildNode{

    void InitLeaf(int first, int n, const BoundingBox& b){
        firstPrimOffset = first;
        nPrimitives = n;
        box = b;
        children[0] = children[1] = nullptr;
    }

    void InitInterior(int axis, BVHBuildNode* c0, BVHBuildNode* c1){
        children[0] = c0;
        children[1] = c1;
        box = BoundingBox::Union(c0->box, c1->box);
        splitAxis = axis;
        nPrimitives = 0;
    }

    BoundingBox box;
    BVHBuildNode* children[2];
    int splitAxis, firstPrimOffset, nPrimitives;
};


BVHAccel::BVHAccel(const std::vector<std::shared_ptr<Primitives>> &p, int _maxPrimsInNode,
                   BVHAccel::SplitMethod _splitMethod)
                   : maxPrimsInNode(_maxPrimsInNode), splitMethod(_splitMethod), primitives(std::move(p)) {
    if(primitives.empty())
        return;


    std::vector<BVHPrimitiveInfo> primitiveInfo(primitives.size());
    for(size_t i = 0; i < primitives.size(); ++i)
        primitiveInfo[i] = {i, primitives[i]->GetBoundingBox()};

    int totalNode = 0;
    std::vector<std::shared_ptr<Primitives>> orderedPrims;
    orderedPrims.reserve(primitives.size());

}

BVHBuildNode *
BVHAccel::recursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfo, int start, int end, int *totalNodes,
                         std::vector<std::shared_ptr<Primitives>> &orderedPrims) {
    BVHBuildNode* node = new BVHBuildNode();
    *(totalNodes)++;
    BoundingBox bounds;
    for(int i = start; i < end; ++i)
        bounds = BoundingBox::Union(bounds, primitiveInfo[i].bounds);

    int nPrimitives = end - start;
    if(nPrimitives == 1){
        int firstPrimOffset = orderedPrims.size();
        for(int i = start; i < end; ++i){
            int primNum = primitiveInfo[i].primitiveNumber;
            orderedPrims.push_back(primitives[primNum]);
        }
        node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
    } else {
        BoundingBox centroidBounds;
        for(int i = start; i < end; ++i)
            centroidBounds = BoundingBox::Union(centroidBounds, primitiveInfo[i].centroid);
        int dim = centroidBounds.MaximumExtent();
        
        int mid = (start + end) / 2;
        if(centroidBounds.pmax[dim] == centroidBounds.pmin[dim]){
            int firstPrimOffset = orderedPrims.size();
            for(int i = start; i < end; ++i){
                int primNum = primitiveInfo[i].primitiveNumber;
                orderedPrims.push_back(primitives[primNum]);
            }
            node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
            return node;
        } else {
            switch (splitMethod) {
                case SplitMethod::Middle: {
                    float pmid = (centroidBounds.pmin[dim] + centroidBounds.pmax[dim]) * 0.5f;
                    BVHPrimitiveInfo *midPtr = std::partition(&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
                                                              [dim, pmid](const BVHPrimitiveInfo &pi) {
                                                                  return pi.centroid[dim] < pmid;
                                                              });
                    mid = midPtr - &primitiveInfo[0];
                    if (mid != start && mid != end)
                        break;
                }
                case SplitMethod::EqualCounts: {
                    mid = (start + end) / 2;
                    std::nth_element(&primitiveInfo[start], &primitiveInfo[mid], &primitiveInfo[end-1]+1,
                                     [dim](const BVHPrimitiveInfo& a, const BVHPrimitiveInfo& b){
                        return a.centroid[dim] < b.centroid[dim];
                    });
                    break;
                }
            }
            node->InitInterior(dim, recursiveBuild(primitiveInfo, start, mid, totalNodes, orderedPrims),
                               recursiveBuild(primitiveInfo, mid, end, totalNodes, orderedPrims));
        }
    }
    return node;
}
