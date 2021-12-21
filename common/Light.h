//
// Created by zeming on 2021/12/21.
//

#ifndef PTRENDERER_LIGHT_H
#define PTRENDERER_LIGHT_H

#include "glm/vec3.hpp"
namespace PTRenderer{
    class Light {
    public:

        enum LightType{
            POINT_LIGHT,
            DIRECTIONAL_LIGHT,
            POLYGON_LIGHT
        };

        Light(const glm::vec3& _color, LightType _type);
        virtual glm::vec3 get_light_dir(const glm::vec3& pos) = 0;
        virtual const glm::vec3& get_light_density(const glm::vec3& pos) const { return base_color; }

    protected:
        LightType type;
        glm::vec3 base_color;

    };


    class PointLight : public Light {

    public:
        PointLight(const glm::vec3 _center, const glm::vec3& _color);
        virtual glm::vec3 get_light_dir(const glm::vec3& pos);
        virtual const glm::vec3& get_light_density(const glm::vec3& pos) const;

    private:
        glm::vec3 center;
        glm::vec3 light_color;

    };

    class DirectionLight : public Light {

    public:
        DirectionLight(const glm::vec3& _direction, const glm::vec3& _color);
        virtual glm::vec3 get_light_dir(const glm::vec3& pos);
        virtual const glm::vec3& get_light_density(const glm::vec3& pos) const;


    private:
        glm::vec3 direction;
        glm::vec3 light_color;

    };


}



#endif //PTRENDERER_LIGHT_H
