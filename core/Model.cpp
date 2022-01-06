//
// Created by zeming on 2021/12/23.
//

#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>
#include <filesystem>

namespace PTRenderer{

    Model::Model(std::string path): directory(path){
//        if(std::filesystem::exist)
        // filesystem to get obj path
        if(!std::filesystem::exists(path))
            std::cerr << "ERROR::ASSIMP::DIRECTORY DOES NOT EXISTS: " << path << std::endl;
        std::string obj_path = "";
        for(const auto& entry : std::filesystem::directory_iterator(path)){
            std::string file_path = entry.path();
            if(file_path.find(".obj") != std::string::npos){
                obj_path = file_path;
                break;
            }
        }
        if(obj_path == ""){
            std::cerr << "Obj file not found : " << path << std::endl;
            exit(1);
        }
        load_model(obj_path);
    }

    void Model::load_model(std::string path) {
        Assimp::Importer importer;
        const aiScene* scene =  importer.ReadFile(path, aiProcess_Triangulate | aiPrimitiveType_TRIANGLE);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        process_node(scene->mRootNode, scene);
    }

    void Model::process_node(aiNode *node, const aiScene *scene) {
        for(unsigned i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            std::vector<Triangle> tri_group = process_mesh(mesh, scene);
            mesh_index++;
            for(auto& tri : tri_group)
                meshes.push_back(tri);
        }

        for(unsigned i = 0; i < node->mNumChildren; i++){
            process_node(node->mChildren[i], scene);
        }
    }

    std::vector<Triangle> Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Triangle> triangles;
        std::cout<< "MODEL::PROCESS_MESH: " << mesh->mNumFaces << std::endl;
        for(unsigned i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
//            std::cout << "aiFace::mNumIndices: " << face.mNumIndices << std::endl;
            assert(face.mNumIndices == 3);
            std::vector<glm::vec3> points;
            for(unsigned j =0; j < face.mNumIndices; j++){
                unsigned index = face.mIndices[j];
                points.push_back(aiVecToGlmVec(mesh->mVertices[index]));
            }
            std::shared_ptr<Material> material = std::make_shared<Material>(color_set[mesh_index % color_set.size()]);
            triangles.emplace_back(points, material);
        }
        return triangles;
    }

    glm::vec3 Model::aiVecToGlmVec(const aiVector3D &v) {
        return glm::vec3(v.x, v.y, v.z);
    }


}