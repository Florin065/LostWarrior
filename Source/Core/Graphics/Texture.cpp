#include <cstdlib>

#include "glad/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/Coordinator.hpp"
#include "Core/Graphics/Texture.hpp"


extern Coordinator gCoordinator;


unsigned int Texture::FromPath(std::string const& path,
                               std::string const& directory)
{
    std::string filename = std::string(path);
    filename = directory + "/" + filename;

    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(),
                                    &width, &height,
                                    &nrComponents, 0);
    if (data)
    {
        GLenum format;
        switch (nrComponents) {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default:
            gCoordinator.LogError("Unknown texture format!");
            exit(EXIT_FAILURE);
        }

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                     format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        gCoordinator.LogError("Texture failed to load at path: ", path);
    }

    stbi_image_free(data);

    return textureId;
}

