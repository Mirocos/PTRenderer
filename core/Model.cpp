//
// Created by zeming on 2021/12/23.
//
#define STB_IMAGE_IMPLEMENTATION
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
            std::string file_path = entry.path().string();
            if(file_path.find(".obj") != std::string::npos){
                obj_path = file_path;
                break;
            }
        }
        if(obj_path == ""){
            std::cerr << "Obj file not found : " << path << std::endl;
            exit(1);
        }
        LoadModel(obj_path);
    }

    void Model::LoadModel(std::string path) {
        Assimp::Importer importer;
        const aiScene* scene =  importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(aiNode *node, const aiScene *scene) {

        for(unsigned int i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++){
            ProcessNode(node->mChildren[i], scene);
        }

    }



    glm::vec3 Model::aiVecToGlmVec(const aiVector3D &v) {
        return glm::vec3(v.x, v.y, v.z);
    }

    Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++){
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            // normal

            if(mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if(mesh->mTextureCoords[0]){
                glm::vec2 texCoords;
                texCoords.x = mesh->mTextureCoords[0][i].x;
                texCoords.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = texCoords;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;

            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        // handle indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j  < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
            }
        }


        glm::vec4 diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
        glm::vec4 specular = glm::vec4(0.f, 0.f, 0.f, 0.f);
        glm::vec4 ambient = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 emission = glm::vec4(0.f, 0.f, 0.f, 0.f);
        float shininess = 0.f;
        float shininessStrength = 1.f;
        float ior = 1.f;
        unsigned int max = 1;

        if(mesh->mMaterialIndex >= 0){
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            aiColor4D aiDiffuse;
            aiColor4D aiSpecular;
            aiColor4D aiAmbient;
            aiColor4D aiEmission;
            if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &aiDiffuse))
            {
                diffuse = aiColor4DToGlmVec4(aiDiffuse);
            }
            if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &aiSpecular))
            {
                specular = aiColor4DToGlmVec4(aiSpecular);
            }
            if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &aiAmbient))
            {
                ambient = aiColor4DToGlmVec4(aiAmbient);
            }
            if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &aiEmission))
            {
                emission = aiColor4DToGlmVec4(aiEmission);
            }

            aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);
            aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &shininessStrength, &max);
            shininess *= shininessStrength;

            aiGetMaterialFloatArray(material, AI_MATKEY_REFRACTI, &ior, &max);

            // 1. diffuse maps
            std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        }

        shared_ptr<Material> material = make_shared<PhongMaterial>(ambient, diffuse, specular, emission, ior, shininess);
        return Mesh(vertices, indices, textures, material);
    }

    std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;

            for(unsigned int j = 0; j < textures_loaded.size(); j++){
                if(std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0){
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }

            if(!skip){
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }

        }
        return textures;
    }

    std::vector<Triangle> Model::GetMesh() {
        std::vector<Triangle> tri_mesh;
        for(int i = 0; i < meshes.size(); i++){
            Mesh mesh = meshes[i];
            auto tris = mesh.GetTriangles();
            tri_mesh.insert(tri_mesh.end(), tris.begin(), tris.end());
        }

        return tri_mesh;
    }

    glm::vec4 Model::aiColor4DToGlmVec4(const aiColor4D &v) {
        return glm::vec4(v.r, v.g, v.b, v.a);
    }


}


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}