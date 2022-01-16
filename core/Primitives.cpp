//
// Created by zeming on 2021/12/17.
//

#include <iostream>
#include "Primitives.h"
const float INTERSECTION_EPSILON = 1e-4;

namespace PTRenderer{

    Primitives::Primitives(const ObjectType &_type, std::shared_ptr<Material> _material)
    : type(_type), material(std::move(_material)){

    }


    Triangle::Triangle(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, std::shared_ptr<Material> _material)
    : Primitives(ObjectType::TRIANGLE, _material), a(_a), b(_b), c(_c){
        calculate_normal();
    }

    Triangle::Triangle(const std::vector<glm::vec3> &points, std::shared_ptr<Material> _material)
    : Primitives(ObjectType::TRIANGLE, _material), a(points[0]), b(points[1]), c(points[2]){
        calculate_normal();
    }

    void Triangle::calculate_normal(){
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        normal = glm::cross(ab, ac);
        normal = glm::normalize(normal);
    }

    Triangle::Triangle(const Triangle &triangle) : Primitives(triangle.type, triangle.material) {
        *this = triangle;
    }

    bool Triangle::intersect(const Ray &ray, Intersection &hit, float tmin) {
       glm::vec3 ro = ray.get_origin();
       glm::vec3 rd = ray.get_direction();
       rd = glm::normalize(rd);
        float length = glm::length(rd);

       glm::mat3 A = get_matA(rd);
       glm::mat3 BETA = get_matBeta(ro, A);
       glm::mat3 GAMMA = get_matGamma(ro, A);
       glm::mat3 T = get_matT(ro ,A);

       float detA = glm::determinant(A);
       float inverse_detA = 1.f / detA;
       float detBETA = glm::determinant(BETA);
//        calculate_det(BETA);
       float detGAMMA = glm::determinant(GAMMA);
//        calculate_det(GAMMA);
       float detT = glm::determinant(T);
//        calculate_det(T);

       float beta = detBETA * inverse_detA;
       float gamma = detGAMMA * inverse_detA;
       float t = detT * inverse_detA;


       // TODO may add Epsilon?
       if(!(beta > 0 && gamma > 0 && beta + gamma < 1.f))
           return false;

       if( t < tmin )
           return false;

       if(t < hit.get_t() + INTERSECTION_EPSILON){
           glm::vec3 hit_point = ro + rd * t;
           hit.set_t(t);
           hit.set_material(material);
           hit.set_intersection(hit_point);
           hit.set_normal(normal);
           return true;
       }

       return false;
    }

    glm::vec3 Triangle::shade() {
        return material->get_diffuse_color();
    }

    glm::mat3 Triangle::get_matA(const glm::vec3 rd) {
        glm::mat3 A;
        A[0][0] = a.x - b.x;
        A[0][1] = a.x - c.x;
        A[0][2] = rd.x;
        A[1][0] = a.y - b.y;
        A[1][1] = a.y - c.y;
        A[1][2] = rd.y;
        A[2][0] = a.z - b.z;
        A[2][1] = a.z - c.z;
        A[2][2] = rd.z;

        return A;
    }

    glm::mat3 Triangle::get_matBeta(const glm::vec3 &ro, const glm::mat3 &A) {
        glm::mat3 BETA(A);
        BETA[0][0] = a.x - ro.x;
        BETA[1][0] = a.y - ro.y;
        BETA[2][0] = a.z - ro.z;
        return BETA;
    }

    glm::mat3 Triangle::get_matGamma(const glm::vec3 &ro, const glm::mat3 &A) {
        glm::mat3 GAMMA(A);
        GAMMA[0][1] = a.x - ro.x;
        GAMMA[1][1] = a.y - ro.y;
        GAMMA[2][1] = a.z - ro.z;
        return GAMMA;
    }

    glm::mat3 Triangle::get_matT(const glm::vec3 &ro, const glm::mat3 &A) {
        glm::mat3 T(A);
        T[0][2] = a.x - ro.x;
        T[1][2] = a.y - ro.y;
        T[2][2] = a.z - ro.z;
        return T;
    }


    Group::Group(int _num) : Primitives(ObjectType::GROUP, std::make_shared<Material>(glm::vec3(1.f, 1.f, 1.f))), num(_num) {
        objects.resize(num);
    }

    bool Group::intersect(const Ray &ray, Intersection &hit, float tmin) {
        bool intersected = false;
        for(unsigned i = 0; i < num; i++){
            if(objects[i]->intersect(ray, hit, tmin)){
                intersected = true;
            }
        }
        return intersected;
    }

    void Group::add_primitive(int index, std::shared_ptr<Primitives> obj) {
        objects[index] = obj;
    }

    Transform::Transform(const glm::mat4 &_m, const std::shared_ptr<Primitives> &_obj)
    : Primitives(ObjectType::TRANSFORM, std::make_shared<Material>(glm::vec3(1.f, 1.f, 1.f))), matrix(_m), obj(_obj){
        inverse_matrix = glm::inverse(matrix);
        inverse_transpose_matrix = glm::transpose(inverse_matrix);
    }

    bool Transform::intersect(const Ray &ray, Intersection &hit, float tmin) {

        auto dir_obj = ray.get_direction();
        auto ori_obj = ray.get_origin();

        auto transform_dir_obj = inverse_matrix * glm::vec4(dir_obj, 0.f);

        auto transform_ori_obj = inverse_matrix * glm::vec4(ori_obj, 1.f);

        Ray r_obj(transform_ori_obj, transform_dir_obj);
        Intersection _h(hit.get_material(), hit.get_hit_point(), hit.get_normal(), hit.get_t());
        bool intersected = false;
        if(obj->intersect(r_obj, _h, tmin)){
            intersected = true;
            auto normal = _h.get_normal();
            auto normal_world = inverse_transpose_matrix * glm::vec4(normal, 0.f);
            normal_world = glm::normalize(normal_world);
            hit.set_normal(normal_world);
            hit.set_intersection(_h.get_hit_point());
            hit.set_t(_h.get_t());
            // TODO add ray as a memeber in Intersection


        }

        return intersected;
    }

    glm::vec3 Transform::shade() {
        return material->get_diffuse_color();
    }
}