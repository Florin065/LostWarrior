#include <stdexcept>
#include <string>
#include <unordered_map>
#include <memory>

#include "Core/Coordinator.hpp"
#include "Core/ResourceManager.hpp"


extern Coordinator gCoordinator;


void ResourceManager::LoadShader(std::string const& name,
                std::string const& vertPath,
                std::string const& fragPath,
                std::string const& geomPath)
{
    if (mShaders.count(name))
    {
        gCoordinator.LogInfo("Shader '", name, "' already set. Returning...");
        return;
    }
    mShaders[name] = std::make_shared<Shader>(vertPath, fragPath, geomPath);
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string const& name)
{
    try {
        return mShaders.at(name);
    } catch (std::out_of_range const& e) {
        gCoordinator.LogError("Non-existent shader '", name, "'");
        exit(EXIT_FAILURE);
    }
}

std::shared_ptr<Model> ResourceManager::GetModel(std::string const& path)
{
    try {
        return mModels.at(path);
    } catch (std::out_of_range const& e) {
        mModels[path] = std::make_shared<Model>(path);
        return mModels[path];
    }
}

unsigned int ResourceManager::LoadTexture(std::string const& path,
                                          std::string const& directory)
{
    if (!mTextures.count(path))
    {
        mTextures.insert({path, Texture::FromPath(path, directory)});
    }
    return mTextures[path];
}

