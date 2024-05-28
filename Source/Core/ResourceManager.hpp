#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/Texture.hpp"
#include "Core/Graphics/Model.hpp"

class ResourceManager
{
public:
    void LoadShader(std::string const& name,
                    std::string const& vertPath,
                    std::string const& fragPath,
                    std::string const& geomPath = "");

    std::shared_ptr<Shader> GetShader(std::string const& name);
    std::shared_ptr<Model> GetModel(std::string const& name);

unsigned int LoadTexture(std::string const& path,
                         std::string const& directory);
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<std::string, std::shared_ptr<Model>> mModels;
    std::unordered_map<std::string, unsigned int> mTextures;
};
