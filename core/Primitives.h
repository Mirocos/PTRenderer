//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_PRIMITIVES_H
#define PTRENDERER_PRIMITIVES_H

#include <vector>
#include "Ray.h"
#include "Intersection.h"
#include "iostream"

namespace PTRenderer{

    class BoundingBox {
    public:
        BoundingBox();
        BoundingBox(const glm::vec3& _pmin, const glm::vec3& _pmax);

        static BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2);
        static BoundingBox Union(const BoundingBox& b, const glm::vec3& p);
        static BoundingBox Intersect(const BoundingBox& b1, const BoundingBox& b2);

        static bool Overlaps(const BoundingBox& b1, const BoundingBox& b2);
        static bool Inside(const glm::vec3& p, const BoundingBox& b);
        static bool InsideExclusive(const glm::vec3& p, const BoundingBox& b);


        static inline BoundingBox Expand(const BoundingBox& b, const glm::vec3& delta);


        glm::vec3 Offset(const glm::vec3& p) const;
        glm::vec3 Lerp(const glm::vec3& t) const;
        glm::vec3 Diagonal() const { return pmax - pmin; }
        float SurfaceArea() const;
        float Volume() const;
        int MaximumExtent() const;

        void BoundingSphere(glm::vec3& center, float& radius);


        glm::vec3 pmin, pmax;
    };

    class Primitives {

    public:

        enum ObjectType{
            TRIANGLE,
            PLANE,
            SPHERE,
            TRIANGLE_MESH,
            BOUNDINGBOX,
            BVH
        };

        Primitives() = delete;
        Primitives(const ObjectType& _type, std::shared_ptr<Material> _material);
        virtual bool intersect(const Ray& ray, Intersection& hit, float tmin) = 0;
        virtual glm::vec3 shade() = 0;
        virtual BoundingBox GetBoundingBox() { return box; };


    protected:
        ObjectType type;
        std::shared_ptr<Material> material;
        BoundingBox box;

    };


    class Triangle : public Primitives{


    public:
        Triangle() = delete;
        Triangle(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c, std::shared_ptr<Material> _material);
        Triangle(const std::vector<glm::vec3>& points, std::shared_ptr<Material> _material);
        Triangle(const Triangle& triangle);

        virtual bool intersect(const Ray& ray, Intersection& hit, float tmin);
        virtual glm::vec3 shade();


        const glm::vec3& x() const { return a; }
        const glm::vec3& y() const { return b; }
        const glm::vec3& z() const { return c; }


    private:

        glm::mat3 get_matA(const glm::vec3 rd);
        glm::mat3 get_matBeta(const glm::vec3& ro, const glm::mat3& A);
        glm::mat3 get_matGamma(const glm::vec3& ro, const glm::mat3& A);
        glm::mat3 get_matT(const glm::vec3& ro, const glm::mat3& A);

        void calculate_normal();
        void calculate_det(const glm::mat3& A){         std::cout << A[0][0] * A[1][1] * A[2][2] + A[0][1] * A[1][2] * A[2][0] + A[0][2] * A[1][0] * A[2][1] \
        - A[0][0] * A[1][2] * A[2][1] - A[0][1] * A[1][0] * A[2][2] - A[0][2] * A[1][1] * A[2][0] << "\n"; }

        glm::vec3 a, b, c;
        glm::vec3 normal;
    };









}




#endif //PTRENDERER_PRIMITIVES_H
