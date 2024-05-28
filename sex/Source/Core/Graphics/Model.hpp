#pragma once

#include <vector>
#include <memory>

#include <assimp/scene.h>

#include "Core/Graphics/Mesh.hpp"


class Model
{
public:
    Model(std::string const& path);

    void Draw(std::shared_ptr<Shader> shader);

private:
    std::vector<std::shared_ptr<Mesh>> mMeshes;
    std::string mDirectory;

    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat,
                                              aiTextureType type,
                                              std::string typeName);
};

