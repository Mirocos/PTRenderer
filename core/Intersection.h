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
        Intersection(std::shared_ptr<Material> _material, glm::vec3 point, glm::vec3 _normal, float _t);
        Intersection(const Intersection& hit);

        void set_intersection(const glm::vec3& point);
        void set_normal(const glm::vec3& _normal);
        void set_t(float _t);
        void set_material(std::shared_ptr<Material> _material) { material = _material; }


        float get_t() { return t; }
        const std::shared_ptr<Material> get_material() const { return material; }
        const glm::vec3& get_hit_point() const { return intersection_point; }
        const glm::vec3& get_normal() const { return normal; }



    private:
        float t;                // current closest t value
        glm::vec3 intersection_point;
        glm::vec3 normal;
        std::shared_ptr<Material> material;

    };
}


#endif //PTRENDERER_INTERSECTION_H
