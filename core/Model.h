//
// Created by zeming on 2021/12/23.
//

#ifndef PTRENDERER_MODEL_H
#define PTRENDERER_MODEL_H

#include <string>
#include <vector>
#include "assimp/scene.h"
#include "Mesh.h"
#include "stb_image.h"

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
namespace PTRenderer {
    class Model {
    public:
        explicit Model(std::string path);
        std::vector<Triangle> GetMesh();

        void Draw(Shader shader){
            for(auto & meshe : meshes){
                meshe.Draw(shader);
            }
        }

    private:

        void LoadModel(std::string path);
        void ProcessNode(aiNode *node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
        glm::vec3 aiVecToGlmVec(const aiVector3D& v);
        glm::vec4 aiColor4DToGlmVec4(const aiColor4D& v);

        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;
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
