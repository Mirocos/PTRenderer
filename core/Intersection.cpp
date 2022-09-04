//
// Created by zeming on 2021/12/17.
//

#include "Intersection.h"
#include "glm/geometric.hpp"

namespace PTRenderer{

    Intersection::Intersection(std::shared_ptr<Material> _material, glm::vec3 point, glm::vec3 _normal, float _t)
    : material(_material), intersection_point(point), normal(_normal), t(_t), happened(false){
        glm::normalize(normal);
    }

    Intersection::Intersection() :material(nullptr), happened(false) {
        normal = glm::vec3(1.f);
        intersection_point = glm::vec3(INFINITY);
        t = INFINITY;
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

    void Intersection::set_normal(const glm::vec3 &_normal) {
        normal = _normal;
    }




}