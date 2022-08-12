//
// Created by zeming on 2022/3/5.
//

#ifndef PTRENDERER_MESH_H
#define PTRENDERER_MESH_H


#include <glad/glad.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include "Primitives.h"
using namespace std;



struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};


struct Texture{
    unsigned int id;
    string type;
    aiString path;

};

class Mesh{
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;


    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        VAO = 0;
#if REAL_TIME_SHOW
        SetupMesh();
#endif
    }


    std::vector<PTRenderer::Triangle> GetTriangles();

    void Draw(Shader &shader);

private:
    void SetupMesh();

    unsigned int VBO, EBO;


};


#endif //PTRENDERER_MESH_H
