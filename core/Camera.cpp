//
// Created by zeming on 2021/12/17.
//

#include <iostream>
#include "Camera.h"
#include "glm/gtx/string_cast.hpp"

namespace PTRenderer {

    Camera::Camera(const glm::vec3& _eye, const glm::vec3& _pos, const glm::vec3& _up) : center(_eye), pos(_pos){
        auto _direction = glm::normalize(_eye - _pos);
        direction = _direction;
        horizontal = glm::cross(_up, direction);
        horizontal = glm::normalize(horizontal);
        up = glm::cross(direction, horizontal);
        up = glm::normalize(up);

        std::cout << "horizontal: " << glm::to_string(horizontal) << std::endl;
        std::cout << "up: " << glm::to_string(up) << std::endl;
        std::cout << "direction: " << glm::to_string(direction) << std::endl;
    }

    void Camera::translate(const glm::vec3 &vec) {
       center += vec;
    }

    glm::vec3 Camera::get_view_dir(const glm::vec3 &p) {
        glm::vec3 V = center - p;
        return glm::normalize(V);
    }

    OrthographicCamera::OrthographicCamera(const glm::vec3& _eye, const glm::vec3& _pos, const glm::vec3& _up,float _size)
    : Camera(_eye, _pos, _up), size(_size){

    }

    Ray OrthographicCamera::generate_ray(const glm::vec2 &point) {
        assert(point.x >= 0.f && point.x <= 1.f);
        assert(point.y >= 0.f && point.y <= 1.f);
        glm::vec3 ray_dir = -direction;
        glm::vec3 ray_ori = center + (point.x - 0.5f) * up * size + (point.y - 0.5f) * horizontal * size;
        return Ray(ray_ori, ray_dir);
    }

    float OrthographicCamera::get_min_t() const {


        return -INFINITY;
    }

    glm::mat4 OrthographicCamera::get_view_mtx() {
        return glm::lookAt(center, center - direction, up);
    }


    PerspectiveCamera::PerspectiveCamera(const glm::vec3& _eye, const glm::vec3& _pos, const glm::vec3& _up, float _angle)
    : Camera(_eye, _pos, _up), angle(_angle){

    }

    Ray PerspectiveCamera::generate_ray(const glm::vec2 &point) {
        assert(point.x >= 0.f && point.x <= 1.f);
        assert(point.y >= 0.f && point.y <= 1.f);
        float size = glm::tan(glm::radians(angle * 0.5f)) * 2.f;
        glm::vec3 ray_ori = center;
        glm::vec3 ray_dir = -direction + (point.x - 0.5f) * up * size + (point.y - 0.5f) * horizontal * size;
        return Ray(ray_ori, ray_dir);
    }

    float PerspectiveCamera::get_min_t() const {
        return 0.f;
    }

    glm::mat4 PerspectiveCamera::get_view_mtx() {
        return glm::lookAt(center, center - direction, up);
    }
}

