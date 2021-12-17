//
// Created by zeming on 2021/12/17.
//

#include "Primitives.h"

namespace PTRenderer{

    Primitives::Primitives(const ObjectType &_type, std::shared_ptr<Material> _material)
    : type(_type), material(_material){

    }


    Triangle::Triangle(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, std::shared_ptr<Material> _material)
    : Primitives(ObjectType::TRIANGLE, _material), a(_a), b(_b), c(_c){
        // TODO calculate normal and barycentric coordinate
    }

    Triangle::Triangle(const Triangle &triangle) : Primitives(triangle.type, triangle.material) {
        *this = triangle;
    }
}