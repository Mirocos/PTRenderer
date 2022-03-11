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

        virtual bool reflectable() { return true; }
        virtual bool refractable() { return false; }
        virtual glm::vec3 get_kd() { return glm::vec3(0.1, 0.2, 0.3); }
        virtual glm::vec3 get_ks() { return glm::vec3(0.f); }
    protected:
        glm::vec3 diffuse_color; // base color = albedo = reflectance
    };


    class PhongMaterial : public Material{

    public:
        PhongMaterial(const glm::vec3& _diffuse_color, const glm::vec3& _specular_color, glm::vec3 _kd, glm::vec3 _ks, float _index_of_refraction, float _exp = 32.f);
        glm::vec3 shade(const glm::vec3& normal, const glm::vec3& light_dir, const glm::vec3& view_dir) override;

        virtual bool reflectable() { return true; }
        virtual bool refractable() { return true; }
        virtual glm::vec3 get_kd() { return kd; }
        virtual glm::vec3 get_ks() { return ks; }
    private:
        glm::vec3 specular_color;
        glm::vec3 kd, ks;
        float exp;
        float index_of_refraction;

    };



    class CookTorrancetaMerial : public Material{
    public:
        CookTorrancetaMerial(const glm::vec3& _diffuse_color, float roughness, float metallic, float index_of_refraction);
        glm::vec3 shade(const glm::vec3& normal, const glm::vec3& light_dir, const glm::vec3& view_dir) override;

    private:
        float chiGGX(float v);
        float GGX_Distribution(const glm::vec3& n, const glm::vec3& h, float alpha);
        float GGX_PartialGeometryTerm(const glm::vec3& v, const glm::vec3& n, const glm::vec3& h, float alpha);

        glm::vec3 Fresnel_Schlick(float cosTheta, float ior);



        float roughness;
        float metallic;
        float index_of_refraction;
    };
}



#endif //PTRENDERER_MATERIAL_H
