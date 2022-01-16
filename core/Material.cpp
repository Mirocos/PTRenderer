//
// Created by zeming on 2021/12/17.
//

#include "Material.h"

namespace PTRenderer{

    Material::Material(const glm::vec3 &_diffuse_color)
    : diffuse_color(_diffuse_color){

    }

    PhongMaterial::PhongMaterial(const glm::vec3 &_diffuse_color, const glm::vec3 &_specular_color, float _exp,
                                 const glm::vec3 &_reflective_color, const glm::vec3 &_transparent_color,
                                 float _index_of_refraction)
    : Material(_diffuse_color), specular_color(_specular_color), exponent(_exp), reflective_color(_reflective_color),
    transparent_color(_transparent_color), index_of_refraction(_index_of_refraction){

    }
}