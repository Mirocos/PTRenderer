#include <iostream>
#include <memory>
#include "common/Image.h"
#include "core/Scene.h"


#define WIDTH 256
#define HEIGHT 256
int main() {
    Vec3f center = Vec3f(0.f, 0.f, 10.f);
    Vec3f up = Vec3f(0.f, 1.f, 0.f);
    Vec3f direction = Vec3f(0.f, 0.f, -1.f);
    float angle = 45.f;

    std::shared_ptr<PTRenderer::Primitives> tri1 = std::make_shared<PTRenderer::Triangle>(glm::vec3(-2.f, -0.2f, 0.f), glm::vec3(2.f, -0.2f, 0.f), glm::vec3(0.f, -2.f, 0.f), std::make_shared<PTRenderer::Material>(glm::vec3(1.f, 0.f, 0.f)));
    std::shared_ptr<PTRenderer::Camera> camera = std::make_shared<PTRenderer::PerspectiveCamera>(center, up, direction, angle);


    std::shared_ptr<PTRenderer::Scene> scene = std::make_shared<PTRenderer::Scene>(camera);
//    std::shared_ptr<PTRenderer::Model> model = std::make_shared<PTRenderer::Model>("../scenes/veach-mis/");
    std::shared_ptr<PTRenderer::Light> light = std::make_shared<PTRenderer::DirectionLight>(glm::vec3(-0.5f, -0.5f, -1.f), glm::vec3(0.9f, 0.9f, 0.9f));

//    scene->add_model(model);
    scene->add_primitives(tri1);
    scene->add_light(light);
    Image image(WIDTH, HEIGHT);
    image.SetAllPixels(glm::vec3(0.f, 0.f, 0.f));
    float total = 1.f * WIDTH * HEIGHT;
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            printf("\rprogress: %f%%", (1.f * i * WIDTH + 1.f * j) / total * 100);
            float u = (float)(1.f * i / WIDTH);
            float v = 1.f - (float)(1.f * j / HEIGHT);
            PTRenderer::Ray ray = scene->generate_ray(glm::vec2(u, v));
            std::shared_ptr<PTRenderer::Material> material = std::make_shared<PTRenderer::Material>(glm::vec3(1.f, 0.f, 0.f));
            glm::vec3 hit_point = glm::vec3(0.f, 0.f, 0.f);
            PTRenderer::Intersection hit(material, hit_point, glm::vec3(0.f, 0.f, 0.f), INFINITY);
            if(scene->intersect(ray, hit, scene->get_min_t())){
                image.SetPixel(j, i, scene->get_color(hit));

            }
        }
    }
    image.SaveTGA("test.tga");
//    int n = 0;

    return 0;
}
