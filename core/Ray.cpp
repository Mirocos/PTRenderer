//
// Created by zeming on 2021/12/17.
//

#include "Ray.h"

namespace PTRenderer{

    Ray::Ray(const glm::vec3 &_origin, const glm::vec3 &_direction): origin(_origin), direction(_direction) {
        direction = glm::normalize(direction);
    }


}