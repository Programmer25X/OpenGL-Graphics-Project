#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "VAO.h";
#include "VBO.h";
#include "EBO.h";


class Mesh
{
private:
	std::vector<Vertex> verticies;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	VertexArrayObject VAO; 
	VertexBufferObject* VBO; 
	ElementBufferObject* EBO;

public:
	Mesh(std::vector<Vertex> &pVerticies, std::vector<GLuint> &pIndicies, std::vector<Texture> &pTextures);
	void SetupMesh();
	void DrawModel(const VertexArrayObject& VAO, const ElementBufferObject& EBO, Shader& shader);

	const std::vector<Vertex> GetVerticies() const { return verticies; }
	const std::vector<GLuint> GetIndicies() const { return indices; }
	const std::vector<Texture> GetTexture() const { return textures; }
	const VertexArrayObject GetVAO() const { return VAO; }
	const VertexBufferObject* GetVBO() const { return VBO; }
	const ElementBufferObject* GetEBO() const { return EBO; }
};

#endif

