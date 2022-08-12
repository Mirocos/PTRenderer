//
// Created by zeming on 2022/3/5.
//

#include "Renderer.h"
#include "glm/gtx/string_cast.hpp"

const unsigned  int SCR_WIDTH = 800;
const unsigned  int SCR_HEIGHT = 800;
const std::string DataDir = "D:\\study\\code\\PTRenderer/shaders/";





void Renderer::draw() {
    std::string vert_path = DataDir+"vert.glsl";
    std::string frag_path = DataDir+"frag.glsl";
    Shader shader(vert_path.c_str(), frag_path.c_str());

    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1.f, 1.f, 100.f);
    glm::mat4 view = scene->get_view_mtx();
    glm::mat4 model = glm::mat4(1.f);
//    model = glm::translate(model, glm::vec3(-1.f, -0.5f, -10.f));


    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        glm::mat4 view = scene->get_view_mtx();
//        std::cout << glm::to_string(view) << std::endl;
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        for(auto& model : scene->models){
            model->Draw(shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void Renderer::init() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Visualize Scene", NULL, NULL);
    if(window == nullptr){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }


    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Renderer::processInput(GLFWwindow *window) {


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 0.5f;
    float delta_x = 0.f, delta_y = 0.f, delta_z = 0.f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        delta_z -= cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        delta_z += cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        delta_x -= cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        delta_x += cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        delta_y -= cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        delta_y += cameraSpeed;
    }

    glm::vec3 translate = glm::vec3(delta_x, delta_y, delta_z);
    scene->translate(translate);
}
