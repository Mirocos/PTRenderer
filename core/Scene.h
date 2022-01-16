//
// Created by zeming on 2022/1/5.
//

#ifndef PTRENDERER_SCENE_H
#define PTRENDERER_SCENE_H
#include <utility>

#include "Model.h"
#include "Light.h"
#include "Camera.h"
namespace PTRenderer{

    class Scene {


    public:
        Scene()  {
            objects.resize(0);
            models.resize(0);
            lights.resize(0);
            camera = nullptr;
        };


        Scene(const std::string& scene_file);

        ~Scene(){
            objects.clear();
            models.clear();
            lights.clear();
        }


        explicit Scene(std::shared_ptr<Camera>  _camera) : camera(std::move(_camera)){
            objects.resize(0);
            models.resize(0);
            lights.resize(0);
        }

        void add_model(const std::shared_ptr<Model>& model);
        void add_models(const std::vector<std::shared_ptr<Model>>& _models);
        void add_light(const std::shared_ptr<Light>& light);
        void add_lights(const std::vector<std::shared_ptr<Light>>& _lights);
        void add_primitives(const std::shared_ptr<Primitives> obj);

        void set_camera(std::shared_ptr<Camera> _camera) { camera = std::move(_camera)}

        glm::vec3 get_color(const Intersection& hit);
        Ray generate_ray(const glm::vec2& point) { return camera->generate_ray(point); }
        float get_min_t() { return camera->get_min_t(); }

        bool intersect(const Ray& ray, Intersection& hit, float tmin);
    private:
        std::vector<std::shared_ptr<Primitives>> objects;
        std::vector<std::shared_ptr<Model>> models;
        std::vector<std::shared_ptr<Light>> lights;
        std::shared_ptr<Camera> camera;
    };

}

#endif //PTRENDERER_SCENE_H
