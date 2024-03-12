#pragma once

#include <functional>

#include "glad/gl.h"

#include "Core/Graphics/Vertex.hpp"
#include "Core/Graphics/Utils.hpp"

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices,
         const GLenum renderMode = GL_TRIANGLES,
         const GLenum glDrawMode = GL_STATIC_DRAW,
         const GLenum normalized = GL_FALSE)
    : mRenderer(__RenderV)
    , mRenderMode(renderMode)
    , mVerticesSize(vertices.size())
    , mIndicesSize(0)
    {
        const GLenum  type         = glGetTypeEnum(vertices[0].position[0]);
        const GLsizei typeSize     = sizeof(vertices[0].position[0]);

        const GLsizei vertexSize   = sizeof(vertices[0]);

        const GLsizei positionSize = sizeof(vertices[0].position) / typeSize;
        const GLsizei colorSize    = sizeof(vertices[0].color)    / typeSize;
        const GLsizei normalSize   = sizeof(vertices[0].normal)   / typeSize;
        const GLsizei texCoordSize = sizeof(vertices[0].texCoord) / typeSize;

        glGenBuffers(1, &mVBO);
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, vertexSize * mVerticesSize,
                     vertices.data(), glDrawMode);

        glVertexAttribPointer(0, positionSize, type, normalized, vertexSize, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, colorSize, type, normalized, vertexSize,
                (void*)(positionSize * typeSize));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, normalSize, type, normalized, vertexSize,
                (void*)((positionSize + colorSize) * typeSize));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, texCoordSize, type, normalized, vertexSize,
                (void*)((positionSize + colorSize + normalSize) * typeSize));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    Mesh(const std::vector<Vertex> &vertices,
         const std::vector<GLuint> &indices,
         const GLenum renderMode = GL_TRIANGLES,
         const GLenum glDrawMode = GL_STATIC_DRAW,
         const GLenum normalized = GL_FALSE)
    : mRenderer(__RenderI)
    , mRenderMode(renderMode)
    , mVerticesSize(vertices.size())
    , mIndicesSize(indices.size())
    {
        const GLenum  type         = glGetTypeEnum(vertices[0].position[0]);
        const GLsizei typeSize     = sizeof(vertices[0].position[0]);

        const GLsizei vertexSize   = sizeof(vertices[0]);

        const GLsizei positionSize = sizeof(vertices[0].position) / typeSize;
        const GLsizei colorSize    = sizeof(vertices[0].color)    / typeSize;
        const GLsizei normalSize   = sizeof(vertices[0].normal)   / typeSize;
        const GLsizei texCoordSize = sizeof(vertices[0].texCoord) / typeSize;

        glGenBuffers(1, &mVBO);
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, vertexSize * mVerticesSize,
                     vertices.data(), glDrawMode);

        glVertexAttribPointer(0, positionSize, type, normalized, vertexSize, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, colorSize, type, normalized, vertexSize,
                (void*)(positionSize * typeSize));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, normalSize, type, normalized, vertexSize,
                (void*)((positionSize + colorSize) * typeSize));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, texCoordSize, type, normalized, vertexSize,
                (void*)((positionSize + colorSize + normalSize) * typeSize));
        glEnableVertexAttribArray(3);

        glGenBuffers(1, &mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                     sizeof(indices[0]) * mIndicesSize,
                     indices.data(), glDrawMode);

        glBindVertexArray(0);
    }

    void Render(void)
    {
        glBindVertexArray(mVAO);
        mRenderer(mRenderMode, mVerticesSize, mIndicesSize);
        glBindVertexArray(0);
    }

    static void __RenderV(GLenum renderMode,
                          GLsizei verticesSize,
                          [[maybe_unused]] GLsizei indicesSize)
    {
        glDrawArrays(renderMode, 0, verticesSize);
    }

    static void __RenderI(GLenum renderMode,
                          [[maybe_unused]] GLsizei verticesSize,
                          GLsizei indicesSize)
    {
        glDrawElements(renderMode, indicesSize, GL_UNSIGNED_INT, nullptr);
    }

private:
    const std::function<void(GLenum, GLsizei, GLsizei)> mRenderer;

    GLuint mVBO;
    GLuint mEBO;
    GLuint mVAO;

    const GLenum  mRenderMode;
    const GLsizei mVerticesSize;
    const GLsizei mIndicesSize;
};

