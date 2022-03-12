#include <iostream>
#include <memory>
#include "common/Image.h"
#include "core/Renderer.h"
#include "core/Transform.h"
#define WIDTH 256
#define HEIGHT 256

#define TEST_CODE 1
int main() {

#if TEST_CODE
//    float data[4][4] {
//            {1, 2, 3, 4},
//            {5, 6, 7, 8},
//            {9, 10, 11, 12},
//            {13, 14, 15, 16}
//    };
//    Transform t(data);
//    t.Print();

    Matrix4x4 m(1.f);
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            printf("%f%s", m[i][j], j == 3? "\n" : " ");
        }
    }
    printf("\n");

#endif


//    Vec3f center = Vec3f(0.f, 0.f, 30.f);
//    Vec3f up = Vec3f(0.f, 1.f, 0.f);
//    Vec3f direction = Vec3f(0.f, 0.f, -1.f);
//    float angle = 45.f;
//
////    std::shared_ptr<PTRenderer::Primitives> tri1 = std::make_shared<PTRenderer::Triangle>(glm::vec3(-2.f, -0.2f, 0.f), glm::vec3(2.f, -0.2f, 0.f), glm::vec3(0.f, -2.f, 0.f), std::make_shared<PTRenderer::Material>(glm::vec3(1.f, 0.f, 0.f)));
//    std::shared_ptr<PTRenderer::Camera> camera = std::make_shared<PTRenderer::PerspectiveCamera>(center, up, direction, angle);
//    std::shared_ptr<PTRenderer::Scene> scene = std::make_shared<PTRenderer::Scene>(camera);
//    Renderer renderer(scene, nullptr);
//    std::shared_ptr<PTRenderer::Model> model = std::make_shared<PTRenderer::Model>("../scenes/veach-mis/");
//    std::shared_ptr<PTRenderer::Light> light = std::make_shared<PTRenderer::DirectionLight>(glm::vec3(-0.5f, -0.5f, -1.f), glm::vec3(0.9f, 0.9f, 0.9f));
//
//    scene->add_model(model);
//    renderer.draw();
////    scene->add_primitives(tri1);
//    scene->add_light(light);
//    Image image(WIDTH, HEIGHT);
//    image.SetAllPixels(glm::vec3(0.f, 0.f, 0.f));
//    float total = 1.f * WIDTH * HEIGHT;
//    for(int y = HEIGHT-1; y >= 0; y--){
//        for(int x = 0; x < WIDTH; x++){
//            printf("\rprogress: %f%%", (1.f * (HEIGHT-y) * WIDTH + 1.f * x) / total * 100);
//            float u = (float)(1.f * x + 0.5f) / (float)(WIDTH);
//            float v = 1.f - (float)(1.f * y+0.5f) / (float)(HEIGHT);
//            PTRenderer::Ray ray = scene->generate_ray(glm::vec2(u, v));
//            std::shared_ptr<PTRenderer::Material> material = std::make_shared<PTRenderer::Material>(glm::vec3(1.f, 0.f, 0.f));
//            glm::vec3 hit_point = glm::vec3(0.f, 0.f, 0.f);
//            PTRenderer::Intersection hit(material, hit_point, glm::vec3(0.f, 0.f, 0.f), INFINITY);
//            if(scene->intersect(ray, hit, scene->get_min_t())){
//                image.SetPixel(HEIGHT - 1 - y, x, scene->get_color(hit));
//
//            }
//        }
//    }
//    image.SaveTGA("veach.tga");
//    glfwTerminate();
    return 0;
}
