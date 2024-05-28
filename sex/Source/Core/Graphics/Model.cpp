#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Graphics/Model.hpp"

#include "Core/Coordinator.hpp"
#include "Core/ResourceManager.hpp"


extern Coordinator gCoordinator;
extern ResourceManager gResourceManager;


Model::Model(std::string const& path)
{
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(path.c_str(), aiProcess_Triangulate
                                      | aiProcess_FlipUVs);

    if (!scene
    ||  scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
    ||  !scene->mRootNode)
    {
        gCoordinator.LogError("Assimp: ", importer.GetErrorString());
        return;
    }
    mDirectory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(std::shared_ptr<Shader> shader)
{
    for (auto const& mesh : mMeshes)
    {
        mesh->Draw(shader);
    }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(ProcessMesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        vertex.position  = glm::vec3(mesh->mVertices[i].x,
                                     mesh->mVertices[i].y,
                                     mesh->mVertices[i].z);
        vertex.normal    = glm::vec3(mesh->mNormals[i].x,
                                     mesh->mNormals[i].y,
                                     mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                         mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps =
            LoadMaterialTextures(material,
                                 aiTextureType_DIFFUSE,
                                 "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps =
            LoadMaterialTextures(material,
                                 aiTextureType_SPECULAR,
                                 "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat,
                                                 aiTextureType type,
                                                 std::string typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = gResourceManager.LoadTexture(str.C_Str(), mDirectory);
        texture.type = typeName;
        textures.push_back(texture);
    }

    return textures;
}
