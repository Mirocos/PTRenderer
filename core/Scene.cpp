//
// Created by zeming on 2022/1/5.
//

#include "Scene.h"

void PTRenderer::Scene::add_model(const std::shared_ptr<Model> &model) {
    models.push_back(model);
    auto mesh = model->GetMesh();
    for(const auto& tri : mesh){
        objects.push_back(std::make_shared<Triangle>(tri));
    }
}

void PTRenderer::Scene::add_models(const std::vector<std::shared_ptr<Model>> &_models) {
    for(const auto& model : _models)
        add_model(model);
}

bool PTRenderer::Scene::intersect(const PTRenderer::Ray &ray, PTRenderer::Intersection &hit, float tmin) {
    bool is_intersected = false;

    // [TODO]  construct BVH acceleration construction
    for(const auto& obj : objects){
        is_intersected |= obj->intersect(ray, hit, tmin);
    }
    return is_intersected;
}

void PTRenderer::Scene::add_light(const std::shared_ptr<Light> &light) {
    lights.push_back(light);
}

void PTRenderer::Scene::add_lights(const std::vector<std::shared_ptr<Light>> &_lights) {
    for(const auto& light : _lights)
        add_light(light);
}

glm::vec3 PTRenderer::Scene::get_color(const Intersection &hit) {
    glm::vec3 color(0.f);
    for(const auto& light : lights){
        glm::vec3 hit_point = hit.get_hit_point();
        glm::vec3 normal = hit.get_normal();
        std::shared_ptr<Material> _material = hit.get_material();
        glm::vec3 density = light->get_light_density(hit_point);
        glm::vec3 light_dir = light->get_light_dir(hit_point);
        float lamb_term = glm::dot(normal, light_dir) < 0? glm::dot(-normal, light_dir) : glm::dot(normal, light_dir);
        color += lamb_term * density * _material->get_diffuse_color() ;
    }
    return color;
}

void PTRenderer::Scene::add_primitives(const std::shared_ptr<Primitives> obj) {
    objects.push_back(obj);
}

glm::vec3 PTRenderer::Scene::trace_ray(const PTRenderer::Ray &ray, float weight, int bounce) {
    if(weight <= cutoff_wight)
        return glm::vec3(0.f);
    if(bounce >= cutoff_bounce)
        return glm::vec3(0.f);


    Intersection hit;
    if(!intersect(ray, hit, get_min_t()))
        return glm::vec3(0.f);

    glm::vec3 L = lights[0]->get_light_dir(hit.get_hit_point());
    glm::vec3 V = camera->get_view_dir(hit.get_hit_point());
    glm::vec3 N = hit.get_normal();
    glm::vec3 color = hit.get_material()->shade(N, L, V);

    auto _m = hit.get_material();

    if(_m->reflectable()){
        glm::vec3 R = reflect(-L, N);
        Ray reflect_ray(hit.get_hit_point(), R);

    }




    return glm::vec3();
}

