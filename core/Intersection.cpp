//
// Created by zeming on 2021/12/17.
//

#include "Intersection.h"

namespace PTRenderer{

    Intersection::Intersection(std::shared_ptr<Material> _material, glm::vec3 &point, float _t)
    : material(_material), intersection_point(point),  t(_t){
    }

    Intersection::Intersection(const Intersection &hit) {
        *this = hit;
    }

    void Intersection::set_intersection(const glm::vec3 &point) {
        intersection_point = point;
    }

    void Intersection::set_t(float _t) {
        t = _t;
    }


}