//
// Created by zeming on 2021/12/17.
//

#include "Camera.h"

namespace PTRenderer {

    Camera::Camera(const glm::vec3 &_center, glm::vec3 &_up, glm::vec3 &_direction) : center(_center){
        _direction = glm::normalize(_direction);
        direction = _direction;
        _up = glm::normalize(_up);
        horizontal = glm::cross(_direction, _up);
        horizontal = glm::normalize(horizontal);
        up = glm::cross(horizontal, _direction);
        up = glm::normalize(up);
    }

    void Camera::translate(const glm::vec3 &vec) {
       center += vec;
    }

    OrthographicCamera::OrthographicCamera(const glm::vec3 &_center, glm::vec3 &_up, glm::vec3 &_direction,float _size)
    : Camera(_center, _up, _direction), size(_size){

    }

    Ray OrthographicCamera::generate_ray(const glm::vec2 &point) {
        assert(point.x >= 0.f && point.x <= 1.f);
        assert(point.y >= 0.f && point.y <= 1.f);
        glm::vec3 ray_dir = direction;
        glm::vec3 ray_ori = center + (point.x - 0.5f) * up * size + (point.y - 0.5f) * horizontal * size;
        return Ray(ray_ori, ray_dir);
    }

    float OrthographicCamera::get_min_t() const {


        return -INFINITY;
    }

    glm::mat4 OrthographicCamera::get_view_mtx() {
        return glm::lookAt(center, center + direction, up);
    }


    PerspectiveCamera::PerspectiveCamera(const glm::vec3 &_center, glm::vec3 &_up, glm::vec3 &_direction, float _angle)
    : Camera(_center, _up, _direction), angle(_angle){

    }

    Ray PerspectiveCamera::generate_ray(const glm::vec2 &point) {
        assert(point.x >= 0.f && point.x <= 1.f);
        assert(point.y >= 0.f && point.y <= 1.f);
        float size = glm::tan(glm::radians(angle * 0.5f)) * 2.f;
        glm::vec3 ray_ori = center;
        glm::vec3 ray_dir = direction + (point.x - 0.5f) * up * size + (point.y - 0.5f) * horizontal * size;
        return Ray(ray_ori, ray_dir);
    }

    float PerspectiveCamera::get_min_t() const {
        return 0.f;
    }

    glm::mat4 PerspectiveCamera::get_view_mtx() {
        return glm::lookAt(center, center + direction, up);
    }
}

