//
// Created by zeming on 2022/3/15.
//

#ifndef PTRENDERER_BVH_H
#define PTRENDERER_BVH_H
#include "Primitives.h"

using namespace PTRenderer;

enum class SplitMethod{
    NAIVE,
    SAH,
    HLBVH,
    Middle,
    EqualCounts
};

class BVHAccel {
public:
    BVHAccel(const std::vector<std::shared_ptr<Primitives>>, int maxPrimsInNode=1, SplitMethod splitMethod=SplitMethod::NAIVE);


private:
    const int maxPrimsInNode;
    const SplitMethod splitMethod;
    std::vector<std::shared_ptr<Primitives>> primitives;
};


#endif //PTRENDERER_BVH_H
