//
// Created by Mirocos on 2022/8/29.
//

#include "Utils.h"

namespace Utils{
    float getUniformRandomFloat(){
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_real_distribution<float> dist(0, 1);
        return dist(rng);
    }
}
