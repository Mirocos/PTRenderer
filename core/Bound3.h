//
// Created by zeming on 2022/8/12.
//

#ifndef PTRENDERER_BOUND3_H
#define PTRENDERER_BOUND3_H
#include "glm/glm.hpp"
#include <array>

using namespace PTRenderer;

class Bound3 {
public:
    glm::vec3 pMin, pMax;


    // constructor
    Bound3();
    Bound3(const glm::vec3& p);
    Bound3(const glm::vec3& p1, const glm::vec3& p2);


    glm::vec3 diagonal() const;
    int maxExtent() const;
    float surfaceArea() const ;
    glm::vec3 centroid();

    Bound3 intersectSet(const Bound3& b);
    glm::vec3 offset(const glm::vec3& p) const;
    bool overlaps(const Bound3& b1, const Bound3& b2);

    bool intersectBound(const Ray& ray, const glm::vec3& invDir, const std::array<int, 3>& dirIsNeg) const;


    static bool inside(const glm::vec3& p, const Bound3& b);
    static Bound3 Union(const Bound3& b1, const Bound3& b2);
    static Bound3 Union(const Bound3& b, const glm::vec3& p);

    inline const glm::vec3& operator[](int i) const { return (i == 0)? pMin : pMax; }




};


#endif //PTRENDERER_BOUND3_H
