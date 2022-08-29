#include <iostream>
#include <memory>
#include "common/Image.h"
#include "core/Renderer.h"
#include "core/Transform.h"
#define WIDTH 1024
#define HEIGHT 1024

#define TEST_CODE 0





void cornell_box(){

}


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


        auto eye = Point3f(1.f, 1.f, 1.f);
        auto center = Point3f(0.f, 1.f, 1.f);
        auto up = Point3f(0.f, 1.f, 0.f);





#endif


    Vec3f center = Vec3f(0.f, 1.1f, 2.5f);
    Vec3f pos = Vec3f(0.f, 1.f, -1.f);
    Vec3f up = Vec3f(0.f, 1.f, 0.f);
    Vec3f direction = Vec3f(0.f, 0.f, -1.f);
    float angle = 60.f;
//
    std::shared_ptr<PTRenderer::Primitives> tri1 = std::make_shared<PTRenderer::Triangle>(glm::vec3(-0.5f, 0.0f, -3.f),
                                                                                          glm::vec3(0.5f, 0.f, -4.f),
                                                                                          glm::vec3(0.f, 1.f, -3.f),
                                                                                          std::make_shared<PTRenderer::Material>(
                                                                                                  glm::vec3(1.f, 0.f,
                                                                                                            0.f)));

    std::shared_ptr<PTRenderer::Primitives> rect1 = std::make_shared<PTRenderer::Rectangle>(glm::vec3(-0.5f, 0.0f, -4.f),
                                                                                            glm::vec3(0.5f, 0.f, -5.f),
                                                                                            glm::vec3(0.5f, 1.f, -4.f),
                                                                                            glm::vec3(-0.5f, 1.f, -4.f),
                                                                                            std::make_shared<PTRenderer::Material>(
                                                                                                    glm::vec3(0.f, 1.f,
                                                                                                              0.f)));

    std::shared_ptr<PTRenderer::Camera> camera = std::make_shared<PTRenderer::PerspectiveCamera>(center, pos, up,
                                                                                                 angle);
//    std::shared_ptr<PTRenderer::Camera> orth_camera = std::make_shared<PTRenderer::OrthographicCamera>(center, pos, up, 8.0);
    std::shared_ptr<PTRenderer::Scene> scene = std::make_shared<PTRenderer::Scene>(camera);
//    Renderer renderer(scene, nullptr);
    std::shared_ptr<PTRenderer::Model> model = std::make_shared<PTRenderer::Model>("../scenes/cornell-box-original/");
    std::shared_ptr<PTRenderer::Light> light = std::make_shared<PTRenderer::DirectionLight>(
            glm::vec3(-0.5f, -0.5f, -1.f), glm::vec3(0.9f, 0.9f, 0.9f));

    scene->add_model(model);
//    renderer.draw();
//    scene->add_primitives(tri1);
//    scene->add_primitives(rect1);
    scene->add_light(light);

    scene->buildBVH();

    Image image(WIDTH, HEIGHT);
    image.SetAllPixels(glm::vec3(0.f, 0.f, 0.f));
    float total = 1.f * WIDTH * HEIGHT;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            if(x == WIDTH / 2 && y == HEIGHT / 2){
                int n = 0;
            }
            float u = (float)(1.f * x + 0.5f) / (float) (WIDTH);
            float v = (float)(1.f * y + 0.5f)/ (float) (HEIGHT);
            PTRenderer::Ray ray = scene->generate_ray(glm::vec2(u, v));
            glm::vec3 pixelColor = glm::vec3(0.f);
            for(int s = 0; s < SPP; ++s){
                std::shared_ptr<PTRenderer::Material> material = std::make_shared<PTRenderer::Material>(
                        glm::vec3(1.f, 0.f, 0.f));
                glm::vec3 hitPoint = glm::vec3(0.f, 0.f, 0.f);
                PTRenderer::Intersection hit(material, hitPoint, glm::vec3(0.f, 0.f, 0.f), INFINITY);
                pixelColor += scene->castRay(ray, hit, scene->get_min_t(), 0);
            }
            pixelColor /= (float)(SPP);
            image.SetPixel(y, x, pixelColor);
        }
    }

    image.SaveTGA("cornell_box.tga");
    glfwTerminate();
    return 0;

}