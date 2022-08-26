//
// Created by zeming on 2022/1/5.
//

#ifndef PTRENDERER_SCENE_H
#define PTRENDERER_SCENE_H
#include <utility>

#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "BVH.h"
namespace PTRenderer{

    class Scene {


    public:
        Scene()  {
            objects.resize(0);
            models.resize(0);
            lights.resize(0);
            camera = nullptr;
            cutoff_wight = 1e-3;
            cutoff_bounce = 3;
        };

        explicit Scene(std::shared_ptr<Camera>  _camera) : camera(std::move(_camera)), bvh(nullptr){
            objects.resize(0);
            models.resize(0);
            lights.resize(0);
        }
        void add_model(const std::shared_ptr<Model>& model);
        void add_models(const std::vector<std::shared_ptr<Model>>& _models);
        void add_light(const std::shared_ptr<Light>& light);
        void add_lights(const std::vector<std::shared_ptr<Light>>& _lights);
        void add_primitives(const std::shared_ptr<Primitives> obj);

        glm::vec3 get_color(const Intersection& hit);


        // camera access
        Ray generate_ray(const glm::vec2& point) { return camera->generate_ray(point); }
        void translate(const glm::vec3& vec) { camera->translate(vec); }

        glm::mat4 get_view_mtx() { return camera->get_view_mtx(); }

        float get_min_t() { return camera->get_min_t(); }

        bool intersect(const Ray& ray, Intersection& hit, float tmin);
        glm::vec3 trace_ray(const Ray&ray, float weight, int bounce);


        void buildBVH();

        std::vector<std::shared_ptr<Model>> models;


    private:
        std::vector<std::shared_ptr<Primitives>> objects;
        std::vector<std::shared_ptr<Light>> lights;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<BVHAccel> bvh;

        float cutoff_wight;
        int cutoff_bounce;



    };

}

#endif //PTRENDERER_SCENE_H
