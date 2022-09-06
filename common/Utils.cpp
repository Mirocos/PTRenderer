//
// Created by Mirocos on 2022/8/29.
//

#include "Utils.h"

namespace Utils{
    const float exposure = 1.f;
    const float gamma = 2.2f;
    const float toneMapFactor = 1.f;
    float getUniformRandomFloat(){
        thread_local std::random_device dev;
        thread_local std::mt19937 rng(dev());
        std::uniform_real_distribution<float> dist(0.f, 1.f);
        return dist(rng);
    }

    float evalLuminance(const glm::vec3 &color) {
        return glm::dot(color, glm::vec3(0.2126f, 0.7152f, 0.722f));
    }

    glm::vec3 toneMapping(const glm::vec3 & hdrColor) {
        auto mapped = hdrColor / (hdrColor + 1.f);
        return mapped;
    }

    glm::vec3 uncharted2_tonemap_partial(glm::vec3 x)
    {
        float A = 0.15f;
        float B = 0.50f;
        float C = 0.10f;
        float D = 0.20f;
        float E = 0.02f;
        float F = 0.30f;
        return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
    }

    glm::vec3 toneMappingFilmic(glm::vec3 v)
    {
        float exposure_bias = 2.0f;
        glm::vec3 curr = uncharted2_tonemap_partial(v * exposure_bias);

        glm::vec3 W = glm::vec3(11.2f);
        glm::vec3 white_scale = glm::vec3(1.0f) / uncharted2_tonemap_partial(W);
        return curr * white_scale;
    }

    std::string preparePath(const std::filesystem::path &path) {
        std::filesystem::path parentPath = path.parent_path();
        if(!std::filesystem::exists(parentPath)){
            std::filesystem::create_directories(parentPath);
        }
        return path.string();
    }
}
