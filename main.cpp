#include <iostream>
#include <memory>
#include <chrono>
#include "core/Renderer.h"
#include "core/Transform.h"

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include <omp.h>
#define HEIGHT 1024
#define WIDTH 1024

using namespace cv;
int main() {


#if CORNELL_BOX
    glm::vec3 center = glm::vec3(0.f, 1.f, 6.8f);
    glm::vec3 pos = glm::vec3(0.f, 1.f, -1.f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    float angle = 19.5f;
#endif
#if VEACH_MIS
    glm::vec3 center = glm::vec3(0.0f, 2.0f, 15.0f);
    glm::vec3 pos = glm::vec3(0.0f, 1.69521f, 14.0476f);
    glm::vec3 up = glm::vec3(0.0f,0.952421f,-0.304787f);
    float angle = 27.3909f;
#endif
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
    std::shared_ptr<PTRenderer::Scene> scene = std::make_shared<PTRenderer::Scene>(camera);

    std::string sceneName = "CornellBox";
    std::shared_ptr<PTRenderer::Model> model = std::make_shared<PTRenderer::Model>("../scenes/"+sceneName+"/");
    std::shared_ptr<PTRenderer::Light> light = std::make_shared<PTRenderer::DirectionLight>(
            glm::vec3(-0.5f, -0.5f, -1.f), glm::vec3(0.9f, 0.9f, 0.9f));

    scene->add_model(model);
    scene->add_light(light);

    scene->buildBVH();
    Mat cvImage(HEIGHT, WIDTH, CV_32FC3);

    float total = 1.f * HEIGHT * WIDTH;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
#pragma omp parallel for
    for (int y = 0; y < WIDTH ; ++y) {
        for (int x = 0; x < HEIGHT; ++x) {
            float u = (float)(1.f * (HEIGHT-1-x) + 0.5f) / (float) (HEIGHT);
            float v = (float)(1.f * y + 0.5f)/ (float) (WIDTH);
            PTRenderer::Ray ray = scene->generate_ray(glm::vec2(u, v));
            glm::vec3 pixelColor = glm::vec3(0.f);
            for(int s = 0; s < SPP; ++s){
                std::shared_ptr<PTRenderer::Material> material = std::make_shared<PTRenderer::Material>(
                        glm::vec3(0.f, 0.f, 0.f));
                glm::vec3 hitPoint = glm::vec3(0.f, 0.f, 0.f);
                PTRenderer::Intersection hit(material, hitPoint, glm::vec3(0.f, 0.f, 0.f), INFINITY);
                pixelColor += scene->castRay(ray, hit, scene->get_min_t(), 0);
            }
            pixelColor /= (float)(SPP);

            cvImage.at<cv::Vec3f>(Point(y,x)) = cv::Vec3f(pixelColor.b, pixelColor.g, pixelColor.r);
        }
    }
    std::filesystem::path prefix(sceneName);
    std::filesystem::path fileName(sceneName +"-spp-"+ to_string(SPP)+"-cosine-is.exr");
    std::filesystem::path finalPath = prefix/fileName;

    cv::imwrite(Utils::preparePath(finalPath), cvImage);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "time: " << elapsed_seconds.count() << std::endl;
    glfwTerminate();
    return 0;

}