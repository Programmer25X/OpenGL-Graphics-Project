#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex>& pVerticies, std::vector<GLuint>& pIndicies, std::vector<Texture>& pTextures)
{
	verticies = pVerticies;
	indices = pIndicies;
	textures = pTextures; 

	SetupMesh();
}

void Mesh::SetupMesh()
{
	VBO = new VertexBufferObject(verticies.data(), verticies.size());
	EBO = new ElementBufferObject(indices.data(), indices.size());

	VBO->bind();
	EBO->bind();

	VertexBufferLayout vertexLayout;
	vertexLayout.pushElement<float>(3); // Vertex Positions 
	vertexLayout.pushElement<float>(3); // Vertex Normals 
	vertexLayout.pushElement<float>(2); // Vertex Texture Coordinates

	LOG_ERRORS(VAO.addBuffer(*VBO, vertexLayout));


}

void Mesh::drawModel(const VertexArrayObject& VAO, const ElementBufferObject& EBO, Shader& shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;

	for (GLuint i = 0; i < textures.size(); i++)
	{
		LOG_ERRORS(glActiveTexture(GL_TEXTURE0 + i));

		std::string number = "";
		std::string name = textures[i].getType();

		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}
		else if (name == "texture_height")
		{
			number = std::to_string(heightNr++);
		}

		shader.setUniform1i(("u_material " + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].getId());
	}

	glActiveTexture(GL_TEXTURE0);

	LOG_ERRORS(VAO.bind());
	LOG_ERRORS(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	LOG_ERRORS(VAO.unbind()); 
}
