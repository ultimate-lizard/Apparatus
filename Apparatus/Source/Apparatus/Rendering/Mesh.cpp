#include "Mesh.h"

#include <glad/glad.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Logger.h"

Mesh::Mesh(const std::string& resourceName, size_t vertexBufferSize, size_t indexBufferSize) :
	Asset(resourceName),
	vbo(0),
	ebo(0),
	materialIndex(0),
	vertexBufferSize(vertexBufferSize),
	indexBufferSize(indexBufferSize)
{

}

Mesh::Mesh(const std::string& resourceName, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int materialIndex) :
	Asset(resourceName),
	vbo(0),
	ebo(0),
	vertices(vertices),
	indices(indices),
	materialIndex(materialIndex),
	vertexBufferSize(0),
	indexBufferSize(0)
{

}

Mesh::~Mesh()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Mesh::init()
{
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if (vertices.size())
	{
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	vao.bind();
	vao.setStride(sizeof(Vertex));
	vao.addAttribute(3);
	vao.addAttribute(2);
	vao.addAttribute(3);
	vao.addAttribute(4);
	vao.finalize();

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 3));
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 5));
	//glEnableVertexAttribArray(2);

	//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 8));
	//glEnableVertexAttribArray(3);

	glCreateBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	if (indices.size())
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	vao.unbind();
}

void Mesh::bind() const
{
	vao.bind();

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

size_t Mesh::getVertexBufferSize() const
{
	return vertexBufferSize;
}

size_t Mesh::getIndexBufferSize() const
{
	return indexBufferSize;
}
