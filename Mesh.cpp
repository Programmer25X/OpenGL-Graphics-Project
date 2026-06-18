#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex>& pVerticies, std::vector<GLuint>& pIndicies, std::vector<Texture>& pTextures)
{
	verticies = pVerticies;
	indicies = pIndicies;
	textures = pTextures; 
}

void Mesh::SetupMesh()
{
	VBO = new VertexBufferObject(verticies.data(), verticies.size());
	EBO = new ElementBufferObject(indicies.data(), indicies.size());

	VBO->bind();
	EBO->bind();

	VertexBufferLayout vertexLayout;
	vertexLayout.pushElement<float>(3); // Vertex Positions 
	vertexLayout.pushElement<float>(3); // Vertex Normals 
	vertexLayout.pushElement<float>(2); // Vertex Texture Coordinates

	LOG_ERRORS(VAO.addBuffer(*VBO, vertexLayout));



}

void Mesh::DrawModel(Shader& shader)
{


}
