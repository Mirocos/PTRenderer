//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_RAY_H
#define PTRENDERER_RAY_H

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
namespace PTRenderer{
    class Ray {
    public:
        Ray() = delete;
        Ray(const glm::vec3& _origin, const glm::vec3& _direction);
        Ray(const Ray& r) { *this = r; }

        const glm::vec3& get_origin() const { return origin; }
        const glm::vec3& get_direction() const { return direction; }

        glm::vec3& get_origin() { return origin; }
        glm::vec3 get_direction() { return direction; }


    private:
        glm::vec3 origin;
        glm::vec3 direction;
    };
}



#endif //PTRENDERER_RAY_H
