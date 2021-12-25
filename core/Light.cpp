//
// Created by zeming on 2021/12/21.
//

#include "Light.h"
#include "glm/geometric.hpp"


namespace PTRenderer {

    Light::Light(const glm::vec3 &_color, Light::LightType _type) : base_color(_color), type(_type) {

    }


    PointLight::PointLight(const glm::vec3 _center, const glm::vec3 &_color)
    : Light(_color, Light::POINT_LIGHT), center(_center), light_color(_color) {

    }

    glm::vec3 PointLight::get_light_dir(const glm::vec3 &pos) {
        glm::vec3 direction = center - pos;
        glm::normalize(direction);
        assert(glm::length(direction) == 1.0);
        return direction;
    }

    glm::vec3 PointLight::get_light_density(const glm::vec3 &pos) const {
        glm::vec3 distance_dir = center - pos;
        float distance = glm::length(distance_dir);

        // TODO handle when its too close
        if(distance < 1e-5)
            return light_color;

        float density = 1.f / (distance * distance);
        return density * light_color;

    }

    DirectionLight::DirectionLight(const glm::vec3 &_direction, const glm::vec3 &_color)
    : Light(_color, Light::DIRECTIONAL_LIGHT), direction(_direction), light_color(_color){
        glm::normalize(direction);
    }

    glm::vec3 DirectionLight::get_light_dir(const glm::vec3 &pos) {
        return direction;
    }

    glm::vec3 DirectionLight::get_light_density(const glm::vec3 &pos) const {
        return light_color;
    }
}