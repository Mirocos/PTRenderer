//
// Created by zeming on 2021/12/17.
//

#include <iostream>

#include "Primitives.h"
const float INTERSECTION_EPSILON = 1e-3;

namespace PTRenderer{

    Primitives::Primitives(const ObjectType &_type, const std::shared_ptr<Material>& _material)
    : type(_type), material(_material), area(){

    }


    Triangle::Triangle(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, std::shared_ptr<Material> _material)
    : Primitives(ObjectType::TRIANGLE, _material), a(_a), b(_b), c(_c){
        calculate_normal();
        bbx = Bound3::Union(Bound3(_a, _b), _c);
    }

    Triangle::Triangle(const std::vector<glm::vec3> &points, std::shared_ptr<Material> _material)
    : Primitives(ObjectType::TRIANGLE, _material), a(points[0]), b(points[1]), c(points[2]){
        calculate_normal();

        bbx = Bound3::Union(Bound3(a, b), c);
    }

    void Triangle::calculate_normal(){
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        normal = glm::cross(ab, ac);
        area = glm::length(normal) * 0.5f;
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
           hit.hit();
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

    void Triangle::sample(Intersection &p, float &pdf) {
        float r1 = Utils::getUniformRandomFloat();
        float r2 = Utils::getUniformRandomFloat();
        float u = sqrtf(r1) * r2;
        float v = sqrtf(r1) * (1 - r2);
        glm::vec3 point = a * (1 - u - v) + b * u + c * v;
        p.set_intersection(point);
        p.set_material(material);
        p.set_normal(normal);
        pdf = 1.f / area;
    }


    Rectangle::Rectangle(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, const glm::vec3 &_d,
                         std::shared_ptr<Material> _material)
                         : Primitives(Primitives::RECTANGLE, _material)
                         , a(_a), b(_b), c(_c), d(_d){
        area = glm::length(glm::cross(b-a, d-a));
        glm::vec3 ab = glm::normalize(b - a);
        glm::vec3 ad = glm::normalize(d - a );
        normal = glm::normalize(glm::cross(ab, ad));
        D = -glm::dot(normal, a);
        bbx = Bound3::Union(Bound3(_a, _b), Bound3(_c, _d));
    }

    bool Rectangle::intersect(const Ray &ray, Intersection &hit, float tmin) {
        auto ro = ray.get_origin();
        auto rd = ray.get_direction();

        if(glm::dot(rd, normal) == 0)
                return false;

        float t = (-D - glm::dot(ro, normal )) / glm::dot(rd, normal);

        if( t < tmin )
            return false;

        auto p = ro + t * rd;


        auto ab = b - a;
        auto bc = c - b;
        auto cd = d - c;
        auto da = a - d;

        auto ap = p - a;
        auto bp = p - b;
        auto cp = p - c;
        auto dp = p - d;

        auto n1 = glm::cross(ab, ap);
        auto n2 = glm::cross(bc, bp);
        auto n3 = glm::cross(cd, cp);
        auto n4 = glm::cross(da, dp);



        if( glm::dot(n1, normal) < 0.f || glm::dot(n2, normal) < 0.f || glm::dot(n3, normal) < 0.f || glm::dot(n4, normal) < 0.f)
            return false;

        if(t < hit.get_t() + INTERSECTION_EPSILON){
            hit.set_t(t);
            hit.set_intersection(p);
            hit.set_material(material);
            hit.set_normal(normal);
            hit.hit();
            return true;
        }






        return false;
    }

    glm::vec3 Rectangle::shade() {
        return material->get_diffuse_color();
    }


}