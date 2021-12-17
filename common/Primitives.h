//
// Created by zeming on 2021/12/17.
//

#ifndef PTRENDERER_PRIMITIVES_H
#define PTRENDERER_PRIMITIVES_H
#include "Ray.h"
#include "Intersection.h"


namespace PTRenderer{

    enum ObjectType{
        TRIANGLE,
        PLANE,
        SPHERE
    };

    class Primitives {
    public:
        Primitives() = delete;
        Primitives(const ObjectType& _type, std::shared_ptr<Material> _material);
        virtual bool intersect(const Ray& ray, Intersection& hit, float tmin) = 0;
        virtual bool shade() = 0;

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
        virtual bool shade();


        const glm::vec3& x() const { return a; }
        const glm::vec3& y() const { return b; }
        const glm::vec3& z() const { return c; }


    private:
        glm::vec3 a, b, c;
        glm::vec3 normal;
        float gamma;
        float beta;
    };

}




#endif //PTRENDERER_PRIMITIVES_H
