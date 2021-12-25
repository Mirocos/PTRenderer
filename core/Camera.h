//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_CAMERA_H
#define PTRENDERER_CAMERA_H


#include "Ray.h"

// TODO hack, temporary type 'Ray', need to implement later as a Class


namespace PTRenderer{
    class Camera {
    public:
        Camera() = delete;
        Camera(const glm::vec3& _center, glm::vec3& _up, glm::vec3& _direction);
        virtual Ray generate_ray(const glm::vec2& point) = 0;
        virtual float getTMin() const = 0;


    protected:
        glm::vec3 center;
        glm::vec3 up;
        glm::vec3 direction;
        glm::vec3 horizontal;
    };




    class OrthographicCamera : public Camera{
    public:
        OrthographicCamera() = delete;
        OrthographicCamera(const glm::vec3& _center, glm::vec3& _up, glm::vec3& _direction, float _size);

        virtual Ray generate_ray(const glm::vec2& point);
        virtual float get_min_t() const;

    private:
        float size;
    };





    /*
     *
     * define a Perspective Camera whose distance from its origin to image panel is 1.f
     * thus view angle could be used to calculate panel size
     *
     *
     * */
    class PerspectiveCamera : public Camera{
    public:
        PerspectiveCamera() = delete;
        PerspectiveCamera(const glm::vec3& _center, glm::vec3& _up, glm::vec3& _direction, float _angle);

        virtual Ray generate_ray(const glm::vec2& point);
        virtual float get_min_t() const;

    private:
        float angle;
    };

}


#endif //PTRENDERER_CAMERA_H
