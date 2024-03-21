#include "Mesh.h"

#include <glad/glad.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Logger.h"

Mesh::Mesh(const std::string& resourceName, long long bufferSize) :
	Resource("Mesh_" + resourceName),
	vao(0),
	vbo(0),
	ebo(0),
	materialIndex(0),
	bufferSize(bufferSize)
{

}

Mesh::Mesh(const std::string& resourceName, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int materialIndex) :
	Resource("Mesh_" + resourceName),
	vao(0),
	vbo(0),
	ebo(0),
	vertices(vertices),
	indices(indices),
	materialIndex(materialIndex)
{

}

Mesh::~Mesh()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::init()
{
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if (vertices.size())
	{
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 5));
	glEnableVertexAttribArray(2);

	glCreateBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	if (indices.size())
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
	}
}

void Mesh::bind() const
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Mesh::setSubData(const std::vector<Vertex>& vertices)
{
	bind();

	this->vertices = vertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(Vertex), this->vertices.data());
}

void Mesh::setSubData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	bind();

	this->vertices = vertices;
	this->indices = indices;

	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(Vertex), this->vertices.data());
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->indices.size() * sizeof(unsigned int), this->indices.data());
}

const std::vector<Vertex>& Mesh::getVertices() const
{
	return vertices;
}

const std::vector<unsigned int>& Mesh::getIndices() const
{
	return indices;
}

unsigned int Mesh::getMaterialIndex() const
{
	return materialIndex;
}

glm::mat4 Mesh::processTransform(aiNode* node)
{
	if (node->mParent)
	{
		aiMatrix4x4 transformation = node->mParent->mTransformation;

		glm::mat4 meshMatrix;
		meshMatrix[0][0] = transformation.a1;
		meshMatrix[0][1] = transformation.a2;
		meshMatrix[0][2] = transformation.a3;
		meshMatrix[0][3] = transformation.a4;

		meshMatrix[1][0] = transformation.b1;
		meshMatrix[1][1] = transformation.b2;
		meshMatrix[1][2] = transformation.b3;
		meshMatrix[1][3] = transformation.b4;

		meshMatrix[2][0] = transformation.c1;
		meshMatrix[2][1] = transformation.c2;
		meshMatrix[2][2] = transformation.c3;
		meshMatrix[2][3] = transformation.c4;

		meshMatrix[3][0] = transformation.d1;
		meshMatrix[3][1] = transformation.d2;
		meshMatrix[3][2] = transformation.d3;
		meshMatrix[3][3] = transformation.d4;
	}

	return glm::mat4(1.0f);
}
