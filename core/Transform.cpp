//
// Created by zeming on 2022/3/12.
//

#include "Transform.h"


Transform Translate(const Vector3f& data){
    Matrix4x4 m(1.f, 0.f, 0.f, data.x,
                0.f, 1.f, 0.f, data.y,
                0.f, 0.f, 1.f, data.z,
                0.f, 0.f, 0.f, 1.f);

    Matrix4x4 mInv(1.f, 0.f, 0.f, -data.x,
                   0.f, 1.f, 0.f, -data.y,
                   0.f, 0.f, 1.f, -data.z,
                   0.f, 0.f, 0.f, 1.f);

    return Transform(m, mInv);

}

Transform Scale(float x, float y, float z){
    Matrix4x4 m(x, 0.f, 0.f, 0.f,
                0.f, y, 0.f, 0.f,
                0.f, 0.f, z, 0.f,
                0.f, 0.f, 0.f, 1.f);

    Matrix4x4 mInv(1.f / x, 0.f, 0.f, 0.f,
                   0.f, 1.f / y, 0.f, 0.f,
                   0.f, 0.f, 1.f / z, 0.f,
                   0.f, 0.f, 0.f, 1.f);

    return Transform(m, mInv);
}

Transform Scale(const Vector3f& data){
    return Scale(data.x, data.y, data.z);
}


Transform RotateX(float theta){
    float sinTheta = glm::sin(glm::radians(theta));
    float cosTheta = glm::cos(glm::radians(theta));
    Matrix4x4 m(1.f, 0.f, 0.f, 0.f,
                0.f, cosTheta, -sinTheta, 0.f,
                -0.f, sinTheta, cosTheta, 0.f,
                0.f, 0.f, 0.f, 1.f);
    return Transform(m, glm::transpose(m));
}

Transform RotateY(float theta){
    float sinTheta = glm::sin(glm::radians(theta));
    float cosTheta = glm::cos(glm::radians(theta));
    Matrix4x4 m(cosTheta, 0.f, sinTheta, 0.f,
                0.f, 1.f, 0.f, 0.f,
                -sinTheta, 0.f, cosTheta, 0.f,
                0.f, 0.f, 0.f, 1.f);

    return Transform(m, glm::transpose(m));
}

Transform RotateZ(float theta){
    float sinTheta = glm::sin(glm::radians(theta));
    float cosTheta = glm::cos(glm::radians(theta));
    Matrix4x4 m(cosTheta, -sinTheta, 0.f, 0.f,
                sinTheta, cosTheta, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f);
}

Transform Rotate(float theta, const Vector3f &axis){
    Vector3f a = glm::normalize(axis);
    float sinTheta = glm::sin(glm::radians(theta));
    float cosTheta = glm::cos(glm::radians(theta));

    Matrix4x4 m(1.f);
    m[0][0] = a.x * a.x + (1.f - a.x * a.x) * cosTheta;
    m[0][1] = a.x * a.y * (1.f - cosTheta) - a.z * sinTheta;
    m[0][2] = a.x * a.z * (1.f - cosTheta) + a.y * sinTheta;
    m[0][3] = 0.f;

    m[1][0] = a.x * a.y * (1.f - cosTheta) + a.z * sinTheta;
    m[1][1] = a.y * a.y + (1.f - a.y * a.y) * cosTheta;
    m[1][2] = a.y * a.z * (1.f - cosTheta) - a.x * sinTheta;
    m[1][3] = 0.f;

    m[2][0] = a.x * a.z * (1.f - cosTheta) - a.y * sinTheta;
    m[2][1] = a.y * a.z * (1.f - cosTheta) + a.x * sinTheta;
    m[2][2] = a.z * a.z + (1.f - a.z * a.z) * cosTheta;
    m[2][3] = 0.f;

    return Transform(m, glm::transpose(m));
}

//Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up){
//    Matrix4x4 cameraToWorld(1.f);
//
//    cameraToWorld[0][3] = pos.x;
//    cameraToWorld[1][3] = pos.y;
//    cameraToWorld[2][3] = pos.z;
//    cameraToWorld[3][3] = 1.f;
//
//    Vector3f dir = glm::normalize(look - pos);
//
//    // TODO robust check
//
//    Vector3f right = glm::normalize(glm::cross(glm::normalize(up), dir));
//    Vector3f newUp = glm::normalize(glm::cross(dir, right));
//
//    cameraToWorld[0][0] = right.x;
//    cameraToWorld[1][0] = right.y;
//    cameraToWorld[2][0] = right.z;
//    cameraToWorld[3][0] = 0.f;
//
//    cameraToWorld[0][1] = newUp.x;
//    cameraToWorld[1][1] = newUp.y;
//    cameraToWorld[2][1] = newUp.z;
//    cameraToWorld[3][1] = 0.f;
//
//    cameraToWorld[0][2] = dir.x;
//    cameraToWorld[1][2] = dir.y;
//    cameraToWorld[2][2] = dir.z;
//    cameraToWorld[3][2] = 0.f;
//
//    return Transform(glm::inverse(cameraToWorld), cameraToWorld);
//}