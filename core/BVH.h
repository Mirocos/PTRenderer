//
// Created by zeming on 2022/3/15.
//

#ifndef PTRENDERER_BVH_H
#define PTRENDERER_BVH_H
#include "Primitives.h"

using namespace PTRenderer;
struct BVHBuildNode;
struct BVHPrimitiveInfo;

class BVHAccel {
public:

    enum class SplitMethod{
        SAH,
        HLBVH,
        Middle,
        EqualCounts
    };

    BVHAccel(const std::vector<std::shared_ptr<Primitives>>& p, int maxPrimsInNode=1, SplitMethod splitMethod=SplitMethod::Middle);

    BVHBuildNode* recursiveBuild(std::vector<BVHPrimitiveInfo>& primitiveInfo, int start, int end, int *totalNodes, std::vector<std::shared_ptr<Primitives>>& orderedPrims);

private:
    const int maxPrimsInNode;
    const SplitMethod splitMethod;
    std::vector<std::shared_ptr<Primitives>> primitives;
};


#endif //PTRENDERER_BVH_H
