//
// Created by zeming on 2022/3/12.
//

#ifndef PTRENDERER_TRANSFORM_H
#define PTRENDERER_TRANSFORM_H


#include <iostream>
#include "glm/ext/matrix_float4x4.hpp"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"

using namespace PTRenderer;

using Matrix4x4 = glm::mat4;
using Normal = glm::vec3;
using Vector3f = glm::vec3;
using Point3f = glm::vec3;

class Transform {
public:
    Transform() {}
    Transform(const float mat[4][4]) {
        m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0],
                      mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1],
                      mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2],
                      mat[3][3]);
        mInv = glm::inverse(m);
    }


    Transform(const Matrix4x4& mat) : m(mat){
        mInv = glm::inverse(mat);
    }

    Transform(const Matrix4x4& m, const Matrix4x4& inv) : m(m), mInv(inv) {}


    void Print(){
//        std::cout << "m: \n" << glm::to_string(m) <<std::endl;
        for(int i = 0; i < 4; ++i){
            for(int j = 0; j < 4; ++j){
                printf("%f%s", m[i][j], j == 3? "\n" : " ");
            }
        }
        printf("\n\n");
        for(int i = 0; i < 4; ++i){
            for(int j = 0; j < 4; ++j){
                printf("%f%s", mInv[i][j], j == 3? "\n" : " ");
            }
        }
        printf("\n");    }

    friend Transform Inverse(const Transform &t) {
        return Transform(t.mInv, t.m);
    }

    friend Transform Transpose(const Transform &t){
        return Transform(glm::transpose(t.m), glm::transpose(t.mInv));
    }

    bool operator==(const Transform& t) const {
        return t.m == m && t.mInv == mInv;
    }

    bool operator!=(const Transform& t) const {
        return t.m != m || t.mInv != mInv;
    }

    bool operator<(const Transform& t2) const {
        for(int i = 0; i < 4; ++i){
            for(int j = 0; j < 4; ++j){
                if(m[i][j] < t2.m[i][j]) return true;
                if(m[i][j] > t2.m[i][j]) return false;
            }
        }
        return false;
    }

    bool IsIdentity() const {
        return m == Matrix4x4(1.f);
    }

    const Matrix4x4& GetMatrix() const { return m; }
    const Matrix4x4& GetInverseMatrix() const { return mInv; }

    bool HasScale() const {
        float la2 = m[0][0] * m[0][0];
        float lb2 = m[1][1] * m[1][1];
        float lc2 = m[2][2] * m[2][2];
#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
        return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
#undef NOT_ONE
    }

    inline Point3f TransformPoint(const Point3f& p) const;
    inline Vector3f TransformVector3f(const Vector3f & v) const;

    inline Normal operator()(const Normal& ) const;
    inline Ray operator()(const Ray& r) const;
    Transform operator*(const Transform& t2) const;

//    bool SwapsHandedness() const;






private:
    glm::mat4 m;
    glm::mat4 mInv;
};


Vector3f Cross(const Vector3f& v1, const Vector3f& v2);

Transform Translate(const Vector3f &delta);
Transform Scale(float x, float y, float z);
Transform Scale(const Vector3f& data);
Transform RotateX(float theta);
Transform RotateY(float theta);
Transform RotateZ(float theta);
Transform Rotate(float theta, const Vector3f &axis);
Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);
Transform Orthographic(float znear, float zfar);
Transform Perspective(float fov, float znear, float zfar);


inline Point3f Transform::TransformPoint(const Point3f& p) const {
    glm::vec4 p_h =  m * glm::vec4(p, 1.f) ;

    // TODO optimize division operation
    return Point3f(p_h.x, p_h.y, p_h.z) / p_h.w;
}

inline Point3f Transform::TransformVector3f(const Vector3f &v) const {
    glm::vec4 v_h = m * glm::vec4(v, 0.f);

    // TODO need to normalize?
    return Point3f(v_h.x, v_h.y, v_h.z);
}

inline Normal Transform::operator()(const Normal &n) const {
    float x = n.x, y = n.y, z = n.z;
    Normal t_n = Normal(mInv[0][0] * x + mInv[1][0] * y + m[2][0] * z,
                        mInv[0][1] * x + mInv[1][1] * y + m[2][1] * z,
                        mInv[0][2] * x + mInv[1][2] * y + m[2][2] * z
                        );
    t_n = glm::normalize(t_n);
    return t_n;

}

inline Ray Transform::operator()(const Ray &r) const {
    Point3f o = TransformPoint(r.get_origin());
    Point3f d = TransformVector3f(r.get_direction());


}
#endif //PTRENDERER_TRANSFORM_H
