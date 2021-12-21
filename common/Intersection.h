//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_INTERSECTION_H
#define PTRENDERER_INTERSECTION_H
#pragma once


#include <memory>
#include "glm/vec3.hpp"
#include "Material.h"

namespace PTRenderer {
    class Intersection {
    public:
        Intersection() = delete;
        Intersection(std::shared_ptr<Material> _material, glm::vec3& point, float _t);
        Intersection(const Intersection& hit);

        void set_intersection(const glm::vec3& point);
        void set_t(float _t);
        float get_t() { return t; }
        void set_material(std::shared_ptr<Material> _material) { material = _material; }

    private:
        float t;                // current closest t value
        glm::vec3 intersection_point;
        std::shared_ptr<Material> material;
    };
}


#endif //PTRENDERER_INTERSECTION_H
