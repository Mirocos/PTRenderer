//
// Created by zeming on 2022/8/12.
//

#include "Bound3.h"
#include "Ray.h"

Bound3::Bound3() {
    float minNum = std::numeric_limits<float>::lowest();
    float maxNum = std::numeric_limits<float>::max();
    pMax = glm::vec3(minNum);
    pMin = glm::vec3(maxNum);
}

Bound3::Bound3(const glm::vec3 &p) : pMin(p), pMax(p) {

}

Bound3::Bound3(const glm::vec3 &p1, const glm::vec3 &p2) {
    pMin = glm::vec3(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
    pMax = glm::vec3(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
}


glm::vec3 Bound3::diagonal() const {

    return pMax - pMin;
}

int Bound3::maxExtent() const {

    glm::vec3 d = diagonal();
    if(d.x > d.y && d.x > d.z)
        return 0;
    else if(d.y > d.z)
        return 1;
    else
        return 2;
}

float Bound3::surfaceArea() const {
    glm::vec3 d = diagonal();
    return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

glm::vec3 Bound3::centroid() {
    return glm::vec3();
}

Bound3 Bound3::intersectSet(const Bound3 &b) {
    return Bound3(glm::vec3(fmax(pMin.x, b.pMin.x),
                            fmax(pMin.y, b.pMin.y),
                            fmax(pMin.z, b.pMin.z)
                            ),
                  glm::vec3(fmin(pMax.x, b.pMax.x),
                                 fmin(pMax.y, b.pMax.y),
                                 fmin(pMax.z, b.pMax.z)));
}

glm::vec3 Bound3::offset(const glm::vec3 &p) const {
    glm::vec3 o = p - pMin;
    if (pMax.x > pMin.x)
        o.x /= pMax.x - pMin.x;
    if (pMax.y > pMin.y)
        o.y /= pMax.y - pMin.y;
    if (pMax.z > pMin.z)
        o.z /= pMax.z - pMin.z;
    return o;
}

bool Bound3::overlaps(const Bound3 &b1, const Bound3 &b2) {
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);

    return (x && y && z);
}

bool Bound3::inside(const glm::vec3 &p, const Bound3 &b) {
    return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
            p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
}

bool Bound3::intersectBound(const Ray &ray, const glm::vec3 &invDir, const std::array<int, 3> &dirIsNeg) const {

    // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
    // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic

    glm::vec3 dir = ray.get_direction();
    glm::vec3 ori = ray.get_origin();
    float tmin =  ((*this)[1-dirIsNeg[0]].x - ori.x) * invDir.x;
    float tmax = ((*this)[dirIsNeg[0]].x - ori.x) * invDir.x;

    float tymin = ((*this)[1-dirIsNeg[1]].y - ori.y) * invDir.y;
    float tymax = ((*this)[dirIsNeg[1]].y - ori.y) * invDir.y;


    if(tmin > tymax || tymin > tmax)
        return false;

    tmin = tmin > tymin? tmin : tymin;
    tmax = tmax < tymax? tmax : tymax;

    float tzmin = ((*this)[1-dirIsNeg[2]].z - ori.z) * invDir.z;
    float tzmax = ((*this)[dirIsNeg[2]].z - ori.z) * invDir.z;

    if(tmin > tzmax || tzmin > tmax)
        return false;

    tmin = tmin > tzmin? tmin : tzmin;
    tmax = tmax < tzmax? tmax : tzmax;

    return true;

}

Bound3 Bound3::Union(const Bound3 &b1, const Bound3 &b2) {
    Bound3 ret;
    ret.pMin = glm::min(b1.pMin, b2.pMin);
    ret.pMax = glm::max(b1.pMax, b2.pMax);
    return ret;
}

Bound3 Bound3::Union(const Bound3 &b, const glm::vec3 &p) {
    Bound3 ret;
    ret.pMin = glm::min(b.pMin, p);
    ret.pMax = glm::max(b.pMax, p);
    return ret;
}
