//
// Created by zeming on 2021/12/17.
//

#include "Primitives.h"
const float INTERSECTION_EPSILON = 1e-4;

namespace PTRenderer{

    Primitives::Primitives(const ObjectType &_type, std::shared_ptr<Material> _material)
    : type(_type), material(_material){

    }


    Triangle::Triangle(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, std::shared_ptr<Material> _material)
    : Primitives(ObjectType::TRIANGLE, _material), a(_a), b(_b), c(_c){
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        normal = glm::cross(ab, ac);
        glm::normalize(normal);
    }

    Triangle::Triangle(const Triangle &triangle) : Primitives(triangle.type, triangle.material) {
        *this = triangle;
    }

    bool Triangle::intersect(const Ray &ray, Intersection &hit, float tmin) {
       glm::vec3 ro = ray.get_origin();
       glm::vec3 rd = ray.get_direction();

       assert(glm::length(rd) == 1.0);

       glm::mat3 A = get_matA(rd);
       glm::mat3 BETA = get_matBeta(ro, A);
       glm::mat3 GAMMA = get_matGamma(ro, A);
       glm::mat3 T = get_matT(ro ,A);

       float inverse_detA = 1.f / glm::determinant(A);
       float detBETA = glm::determinant(BETA);
       float detGAMMA = glm::determinant(GAMMA);
       float detT = glm::determinant(T);

       float beta = detBETA * inverse_detA;
       float gamma = detGAMMA * inverse_detA;
       float t = detT * inverse_detA;


       // TODO may add Epsilon?
       if(beta > 0 && gamma > 0 && beta + gamma < 1.f)
           return false;

       if( t > tmin )
           return false;

       if(t < hit.get_t() + INTERSECTION_EPSILON){
           glm::vec3 hit_point = ro + rd * t;
           hit.set_t(t);
           hit.set_material(material);
           hit.set_intersection(hit_point);
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


}