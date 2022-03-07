//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_MATERIAL_H
#define PTRENDERER_MATERIAL_H

#include "glm/vec3.hpp"

namespace PTRenderer{
    class Material {
    public:
        Material() : diffuse_color(glm::vec3(1.f)){}
        Material(const glm::vec3& _diffuse_color);

        const glm::vec3& get_diffuse_color() { return diffuse_color; }
        virtual glm::vec3 shade(const glm::vec3& normal, const glm::vec3& light_dir, const glm::vec3& view_dir) = 0;
    protected:
        glm::vec3 diffuse_color;
    };


    class PhongMaterial : public Material{
    public:
        PhongMaterial(const glm::vec3& _diffuse_color, const glm::vec3& _specular_color, float _index_of_refraction, float _exp = 32.f);
        glm::vec3 shade(const glm::vec3& normal, const glm::vec3& light_dir, const glm::vec3& view_dir) override;


    private:
        glm::vec3 specular_color;
        float exp;
        float index_of_refraction;
    };
}



#endif //PTRENDERER_MATERIAL_H
