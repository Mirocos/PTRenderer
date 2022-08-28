//
// Created by zeming on 2022/3/7.
//

#ifndef PTRENDERER_UTILS_H
#define PTRENDERER_UTILS_H

#include <algorithm>
#include <random>
namespace Utils {
    template<typename T>
    T clamp(T value, T minv, T maxv) {
        return std::max(std::min(value, maxv), minv);
    }

    template<typename T>
    T lerp(T lh, T rh, float t){
        return (1 - t) * lh + t * rh;
    }

    float getUniformRandomFloat();

}

#endif //PTRENDERER_UTILS_H
