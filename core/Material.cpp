//
// Created by zeming on 2021/12/17.
//

#include "Material.h"
#include "glm/geometric.hpp"
#include "Utils.h"
namespace PTRenderer{

    Material::Material(const glm::vec3 &_diffuse_color)
    : diffuse_color(_diffuse_color){

    }

    PhongMaterial::PhongMaterial(const glm::vec3 &_diffuse_color, const glm::vec3 &_specular_color,  float _index_of_refraction, float _exp)
        : Material(_diffuse_color), specular_color(_specular_color),  index_of_refraction(_index_of_refraction), exp(_exp){

    }

    glm::vec3 PhongMaterial::shade(const glm::vec3 &N, const glm::vec3 &L, const glm::vec3 &V) {
        /*
         *  suppose L is from light to hit point
         *
         * */

        float NdotL = Utils::clamp<float>(glm::dot(N, L), 0.f, 1.f); // cosine between N & L

        glm::vec3 H =  glm::normalize(L + V);
        float NdotH =  Utils::clamp<float>(glm::dot(N, H), 0.f, 1.f);


        return  NdotL * diffuse_color + NdotH * std::pow(NdotH, 32.f) * specular_color;
    }
}