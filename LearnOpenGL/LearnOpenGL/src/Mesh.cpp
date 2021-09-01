#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	//for (unsigned int i = 0; i < textures.size(); i++) {
	//	GLCall(glActiveTexture(GL_TEXTURE0 + i));

	//	std::string number;
	//	std::string name = textures[i].type;
	//	if (name == "texture_diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "texture_specular")
	//		number = std::to_string(specularNr++);

	//	shader.SetInt(("u_material." + name + number).c_str(), i);
	//	GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	//}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0].id);
	shader.SetInt("u_material.texture_diffuse1", 0);

	GLCall(glActiveTexture(GL_TEXTURE0));

	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
}

void Mesh::SetupMesh()
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));
	
	GLCall(glBindVertexArray(VAO));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));
	
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
	
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord)));
	
	GLCall(glBindVertexArray(0));
}
