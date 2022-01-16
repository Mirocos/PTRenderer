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



    class PhongMaterial : public Material{
    public:
        PhongMaterial(const glm::vec3& _diffuse_color, const glm::vec3& _specular_color, float _exp, const glm::vec3& _reflective_color, const glm::vec3& _transparent_color, float _index_of_refraction);


    private:
        glm::vec3 specular_color;
        glm::vec3 reflective_color;
        glm::vec3 transparent_color;


        float exponent;
        float index_of_refraction;
    };
}



#endif //PTRENDERER_MATERIAL_H
