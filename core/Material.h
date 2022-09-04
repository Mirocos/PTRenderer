//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_MATERIAL_H
#define PTRENDERER_MATERIAL_H

#include "glm/vec3.hpp"
#include "Macrodefine.h"
namespace PTRenderer{
    class Material {

    public:
        Material() : diffuse_color(glm::vec3(1.f)), emission_color(glm::vec3(0.f)){}
        explicit Material(const glm::vec3& _diffuse_color, const glm::vec3& emission=glm::vec3(0.f));

        const glm::vec3& get_diffuse_color() { return diffuse_color; }
        virtual glm::vec3 shade(const glm::vec3& normal, const glm::vec3& light_dir, const glm::vec3& view_dir) { return diffuse_color; };
        virtual bool isEmissive() { return emission_color != glm::vec3(0.f); }
        virtual glm::vec3 get_kd() { return glm::vec3(0.1, 0.2, 0.3); }
        virtual glm::vec3 get_ks() { return glm::vec3(0.f); }
        virtual glm::vec3 getEmission() { return emission_color; }
        virtual glm::vec3 sample(const glm::vec3& N, float& pdf) { return glm::vec3(0.f); } ;
        virtual glm::vec3 eval(const glm::vec3& Wi, const glm::vec3& Wo, const glm::vec3& N) { return glm::vec3(0.f); } ;

        glm::vec3 toWorld(const glm::vec3& a, const glm::vec3& N);

    protected:
        glm::vec3 diffuse_color; // base color = albedo = reflectance
        glm::vec3 emission_color;
    };


    class PhongMaterial : public Material{

    public:

        PhongMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& emission, float ior, float shininess);
        glm::vec3 shade(const glm::vec3& normal, const glm::vec3& light_dir, const glm::vec3& view_dir) override;



        glm::vec3 get_kd() override { return kd; }
        glm::vec3 get_ks() override { return ks; }
        glm::vec3 sample(const glm::vec3& N, float& pdf) override;
        glm::vec3 eval(const glm::vec3& Wi, const glm::vec3& Wo, const glm::vec3& N) override;
    private:
        glm::vec3 specular_color;
        glm::vec3 ka,kd, ks;
        float shininess;
        float index_of_refraction;

    };



    class CookTorrancetaMerial : public Material{
    public:
        CookTorrancetaMerial(const glm::vec3& _diffuse_color, float roughness, float metallic, float index_of_refraction): Material(){}
        glm::vec3 shade(const glm::vec3& normal, const glm::vec3& light_dir, const glm::vec3& view_dir) override;
        glm::vec3 sample(const glm::vec3& N, float& pdf) override { return glm::vec3(0.f); }
        glm::vec3 eval(const glm::vec3& Wi, const glm::vec3& Wo, const glm::vec3& N) override { return glm::vec3(0.f); }
    private:
        float chiGGX(float v);
        float GGX_Distribution(const glm::vec3& n, const glm::vec3& h, float alpha);
        float GGX_PartialGeometryTerm(const glm::vec3& v, const glm::vec3& n, const glm::vec3& h, float alpha);
        glm::vec3 Fresnel_Schlick(float cosTheta, float ior);


        float roughness{};
        float metallic{};
        float index_of_refraction{};
    };
}



#endif //PTRENDERER_MATERIAL_H
