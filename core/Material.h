//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_MATERIAL_H
#define PTRENDERER_MATERIAL_H

#include "glm/vec3.hpp"

namespace PTRenderer{
    class Material {
    public:
        Material(const glm::vec3& _diffuse_color);

        const glm::vec3& get_diffuse_color() { return diffuse_color; }

    protected:
        glm::vec3 diffuse_color;
    };
}



#endif //PTRENDERER_MATERIAL_H
