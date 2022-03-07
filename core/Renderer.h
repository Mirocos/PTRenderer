//
// Created by zeming on 2022/3/5.
//

#ifndef PTRENDERER_RENDERER_H
#define PTRENDERER_RENDERER_H
#include "Scene.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
using namespace PTRenderer;

extern const std::string DataDir;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);



class Renderer {
public:
    explicit Renderer(const std::shared_ptr<Scene> _scene, GLFWwindow* _window) : scene(_scene), window(_window) {
        init();
    }

    void draw();


private:

    void init();
    void processInput(GLFWwindow *window);
    std::shared_ptr<Scene> scene;
    GLFWwindow* window;
};


extern Renderer renderer;

#endif //PTRENDERER_RENDERER_H
