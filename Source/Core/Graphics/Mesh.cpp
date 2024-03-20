#include <cstddef>

#include "Core/Graphics/Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<GLuint> indices,
           std::vector<Texture> textures)
    : vertices(vertices)
    , indices(indices)
    , textures(textures)
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
                 &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0); 
}

void Mesh::Draw(std::shared_ptr<Shader> shader)
{
    GLuint diffuseNr  = 1;
    GLuint specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }

        shader->SetUniform("material." + name + number, i);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
