//
// Created by zeming on 2022/3/5.
//

#include "Mesh.h"

void Mesh::SetupMesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Normal));

//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoords));
//
//
//    glEnableVertexAttribArray(3);
//    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Tangent));
//
//    glEnableVertexAttribArray(4);
//    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Bitangent));

    glBindVertexArray(0);

}

void Mesh::Draw(Shader &shader){
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for(unsigned int i = 0; i < textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures[i].type;

        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        else if(name == "texture_height")
            number = std::to_string(heightNr++);
        auto id = glGetUniformLocation(shader.ID, (name + number).c_str());
        glUniform1i(id, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    glActiveTexture(GL_TEXTURE0);

}

std::vector<PTRenderer::Triangle> Mesh::GetTriangles() {
    std::vector<PTRenderer::Triangle> tris;
    for(int i = 0; i < indices.size(); i += 3){
        std::vector<glm::vec3> tri{vertices[indices[i]].Position, vertices[indices[i+1]].Position, vertices[indices[i+2]].Position};
        tris.emplace_back(tri, material);
    }
    return tris;
}
