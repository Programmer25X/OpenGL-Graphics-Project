#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "Mesh.h"

class Model
{
public:
	Model(char *path)
	{
		loadModel(path); 
	}

	void draw(const VertexArrayObject& VAO, const ElementBufferObject& EBO, Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory; 

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string typeName);


};

#endif 

