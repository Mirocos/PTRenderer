//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_PRIMITIVES_H
#define PTRENDERER_PRIMITIVES_H
#include "Ray.h"
#include "Intersection.h"


namespace PTRenderer{

    class Primitives {

    public:
        enum ObjectType{
            TRIANGLE,
            PLANE,
            SPHERE
        };

        Primitives() = delete;
        Primitives(const ObjectType& _type, std::shared_ptr<Material> _material);
        virtual bool intersect(const Ray& ray, Intersection& hit, float tmin) = 0;
        virtual glm::vec3 shade() = 0;

    protected:
        ObjectType type;
        std::shared_ptr<Material> material;

    };


    class Triangle : public Primitives{


    public:
        Triangle() = delete;
        Triangle(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c, std::shared_ptr<Material> _material);
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

        glm::vec3 a, b, c;
        glm::vec3 normal;
    };

}




#endif //PTRENDERER_PRIMITIVES_H
