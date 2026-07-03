#include "Model.h"

#include <iostream>
#include <format>

void Model::draw(const VertexArrayObject& VAO, const ElementBufferObject& EBO, Shader& shader)
{
    for (GLuint i = 0; i < meshes.size(); i++)
    {
        meshes[i].drawModel(VAO, EBO, shader);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::format("ERROR::ASSIMP:: {}", import.GetErrorString()); 
        return;
    }
    else
    {
        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene); 
    }
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene)); 
    }

    for (GLuint j = 0; j < node->mNumChildren; j++)
    {
        processNode(node->mChildren[j], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 tempVector = glm::vec3(0.0f, 0.0f, 0.0f);

        tempVector.x = mesh->mVertices[i].x;
        tempVector.y = mesh->mVertices[i].y;
        tempVector.z = mesh->mVertices[i].z;
        vertex.position = tempVector; 

        if (mesh->HasNormals())
        {
            tempVector.x = mesh->mNormals[i].x;
            tempVector.y = mesh->mNormals[i].y;
            tempVector.z = mesh->mNormals[i].z;
            vertex.normal = tempVector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 tempTextureVector = glm::vec2(0.0f, 0.0f);
            tempTextureVector.x = mesh->mTextureCoords[0][i].x;
            tempTextureVector.y = mesh->mTextureCoords[0][i].y;
            vertex.textureCoordinates = tempTextureVector;

            tempVector.x = mesh->mTangents[i].x;
            tempVector.y = mesh->mTangents[i].y;
            tempVector.z = mesh->mTangents[i].z;
            vertex.tangent = tempVector; 

            tempVector.x = mesh->mBitangents[i].x;
            tempVector.y = mesh->mBitangents[i].y;
            tempVector.z = mesh->mBitangents[i].z;

        }
        else
        {
            vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (GLuint j = 0; j < mesh->mNumFaces; j++)
    {
        aiFace face = mesh->mFaces[j];
        
        for (GLuint k = 0; k < face.mNumIndices; k++)
        {
            indices.push_back(face.mIndices[k]); 
        }
    }
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string typeName)
{
    return std::vector<Texture>();
}


