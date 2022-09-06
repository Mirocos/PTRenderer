//
// Created by zeming on 2022/1/5.
//

#include "Scene.h"
#include "glm/gtx/string_cast.hpp"

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


    if(bvh){
        hit = bvh->Intersect(ray);
        is_intersected = hit.happened;
    }
    else {
        for (const auto &obj: objects) {
            is_intersected |= obj->intersect(ray, hit, tmin);
        }
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






    return glm::vec3();
}

void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = std::make_shared<BVHAccel>(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

glm::vec3 Scene::castRay(const Ray &ray, Intersection &hit, float tmin, int bounce) {
    intersect(ray, hit, tmin);

    if(hit.happened){
        if(hit.get_material()->isEmissive()){
            if(bounce == 0)
                return hit.get_material()->getEmission();
            else
                return glm::vec3(0.f);
        }

/*        glm::vec3 hitPoint = hit.get_hit_point();
        glm::vec3 N = hit.get_normal();
        glm::vec3 lWo = -ray.get_direction();
        float lpdf;
        glm::vec3 lWi = glm::normalize(hit.get_material()->sample(lWo, N, lpdf));

        Ray shadowRay(hitPoint, lWi);
        Intersection shadowHit;
        intersect(shadowRay, shadowHit, tmin);
        glm::vec3 directL = glm::vec3(0.f);
        if(shadowHit.happened && shadowHit.get_material()->isEmissive()) {
            glm::vec3 lN = shadowHit.get_normal();
            float visibility = glm::dot(lN, lWi) < 0.f? 1.f : 0.f;
            directL = visibility * shadowHit.get_material()->getEmission() * hit.get_material()->eval(lWi, lWo, N) * glm::dot(lWi, N) /
                    lpdf;

        }*/

        glm::vec3 Wo = -ray.get_direction();

        Intersection lightInter;
        float lightPdf;
        sampleLight(lightInter, lightPdf);

        glm::vec3 hitPoint = hit.get_hit_point();
        glm::vec3 N = hit.get_normal();
        glm::vec3 point2Light = glm::normalize(lightInter.get_hit_point() - hitPoint);
        Ray ray2Light(hitPoint, point2Light);
        Intersection nextHit;
        intersect(ray2Light, nextHit, tmin);
        glm::vec3 distanceVec = lightInter.get_hit_point() - nextHit.get_hit_point();
        glm::vec3 lightPath = lightInter.get_hit_point() - hitPoint;
        float distance2 = glm::dot(lightPath, lightPath);
        glm::vec3 directL = glm::vec3(0.f);
        if(nextHit.happened && nextHit.get_material()->isEmissive()){
            directL = hit.get_material()->eval(point2Light, Wo, N)* lightInter.get_material()->getEmission() * fmax(glm::dot(N, point2Light), 0.f) * fmax(glm::dot(lightInter.get_normal(), -point2Light),0.f) / distance2 / lightPdf;
        }


        glm::vec3 indirectL = glm::vec3(0.f);
        if(Utils::getUniformRandomFloat() < RussianRoulette){
            float pdf;
            glm::vec3 Wi = glm::normalize(hit.get_material()->sample(Wo, N, pdf));
            if(pdf > 0.f){
                Ray newRay(hitPoint, Wi);
                Intersection newHit;
                indirectL =  castRay(newRay, newHit, tmin, bounce+1) ;
                if(glm::dot(newHit.get_normal(), Wi) < 0.f)
                    indirectL *= hit.get_material()->eval(Wi, Wo, N) * fmax(glm::dot(Wi, N), 0.f) / pdf / RussianRoulette;
                else
                    indirectL = glm::vec3(0.f);
            }
        }


        return  directL + indirectL;

    }

    return glm::vec3(0.f);
}

float Scene::sampleLight(Intersection &inter, float &pdf) {
    float sumArea = 0.f;
    for(const auto& obj : objects){
        if(obj->hasEmission())
            sumArea += obj->getArea();
    }

    float u = Utils::getUniformRandomFloat();
    float targetArea = u * sumArea;
    float emitSumArea = 0.f;
    for(const auto& obj : objects){
        if(obj->hasEmission()){
            emitSumArea += obj->getArea();
            if(targetArea <= emitSumArea){
                obj->sample(inter, pdf);
                pdf *= obj->getArea() / sumArea;
                break;
            }
        }
    }
    return sumArea;

}

