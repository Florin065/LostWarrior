#pragma once

#include <vector>
#include <memory>

#include "glad/gl.h"

#include "Core/Graphics/Vertex.hpp"
#include "Core/Graphics/Texture.hpp"
#include "Core/Graphics/Shader.hpp"

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices,
         std::vector<GLuint> indices,
         std::vector<Texture> textures);

    void Draw(std::shared_ptr<Shader> shader);

private:
    GLuint mVAO, mVBO, mEBO;
};

