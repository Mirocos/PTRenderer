//
// Created by zeming on 2022/3/7.
//

#ifndef PTRENDERER_UTILS_H
#define PTRENDERER_UTILS_H

namespace Utils {
    template<typename T>
    T clamp(T value, T minv, T maxv) {
        return max(min(value, maxv), minv);
    }

}

#endif //PTRENDERER_UTILS_H
