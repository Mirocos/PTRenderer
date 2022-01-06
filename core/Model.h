//
// Created by zeming on 2021/12/23.
//

#ifndef PTRENDERER_MODEL_H
#define PTRENDERER_MODEL_H

#include <string>
#include <vector>
#include "Primitives.h"
#include "assimp/scene.h"

namespace PTRenderer {
    class Model {
    public:
        Model(std::string path);
        const std::vector<Triangle>& get_mesh() const { return meshes; }

    private:

        void load_model(std::string path);
        void process_node(aiNode *node, const aiScene* scene);
        std::vector<Triangle> process_mesh(aiMesh *mesh, const aiScene *scene);

        glm::vec3 aiVecToGlmVec(const aiVector3D& v);

        std::vector<Triangle> meshes;
        std::string directory;
        std::vector<glm::vec3> color_set{
            glm::vec3(1.f, 1.f, 1.f),
            glm::vec3(0.f, 1.f, 1.f),
            glm::vec3(1.f, 0.f, 1.f),
            glm::vec3(1.f, 1.f, 0.f),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(1.f, 0.f, 0.f),
        };
        int mesh_index = 0;

    };
}


#endif //PTRENDERER_MODEL_H
