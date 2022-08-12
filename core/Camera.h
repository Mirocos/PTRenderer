//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_CAMERA_H
#define PTRENDERER_CAMERA_H


#include "Ray.h"




namespace PTRenderer{
    class Camera {
    public:
        Camera() = delete;
        Camera(const glm::vec3& _eye, const glm::vec3& _pos, const glm::vec3& _up);
        virtual Ray generate_ray(const glm::vec2& point) = 0;
        virtual float get_min_t() const = 0;

        virtual glm::mat4 get_view_mtx() = 0;
        virtual void translate(const glm::vec3& vec);


        glm::vec3 get_view_dir(const glm::vec3& p);
    protected:
        glm::vec3 center;
        glm::vec3 pos;
        glm::vec3 up;
        glm::vec3 direction;
        glm::vec3 horizontal;

    };




    class OrthographicCamera : public Camera{
    public:
        OrthographicCamera() = delete;
        OrthographicCamera(const glm::vec3& _eye, const glm::vec3& _pos, const glm::vec3& _up, float _size);

        virtual Ray generate_ray(const glm::vec2& point);
        virtual float get_min_t() const;
        virtual glm::mat4 get_view_mtx();

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
        PerspectiveCamera(const glm::vec3& _eye, const glm::vec3& _pos, const glm::vec3& _up, float _angle);

        virtual Ray generate_ray(const glm::vec2& point);
        virtual float get_min_t() const;
        virtual glm::mat4 get_view_mtx();

    private:
        float angle;
    };

}


#endif //PTRENDERER_CAMERA_H
