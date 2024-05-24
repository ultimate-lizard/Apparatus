#include "Mesh.h"

#include <glad/glad.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Logger.h"

Mesh::Mesh(std::unique_ptr<VertexArrayObject> vao, int vertexBufferSize, int indexBufferSize) :
	vao(std::move(vao)),
	vbo(0),
	ebo(0),
	materialIndex(0),
	vertexBufferSize(vertexBufferSize),
	indexBufferSize(indexBufferSize)
{

}

Mesh::Mesh(std::unique_ptr<VertexArrayObject> vao, std::shared_ptr<VertexBufferInterface> vertices, const std::vector<unsigned int>& indices, unsigned int materialIndex) :
	vao(std::move(vao)),
	vbo(0),
	ebo(0),
	vertexBuffer(vertices),
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

	assert(vao);
	vao->bind();
	vao->finalize();

	if (vertexBuffer)
	{
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer->getDataSize(), vertexBuffer->getData(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

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

	vao->unbind();
}

void Mesh::bind() const
{
	assert(vao);
	vao->bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Mesh::setSubData(std::shared_ptr<VertexBufferInterface> vertices)
{
	bind();

	vertexBuffer = vertices;

	if (vertexBuffer)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer->getDataSize(), vertexBuffer->getData());
	}
}

void Mesh::setSubData(std::shared_ptr<VertexBufferInterface> vertices, const std::vector<unsigned int>& indices)
{
	bind();

	vertexBuffer = vertices;
	this->indices = indices;

	if (vertexBuffer)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer->getDataSize(), vertexBuffer->getData());
	}

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->indices.size() * sizeof(unsigned int), this->indices.data());
}

const std::vector<unsigned int>& Mesh::getIndices() const
{
	return indices;
}

unsigned int Mesh::getMaterialIndex() const
{
	return materialIndex;
}

int Mesh::getVertexBufferSize() const
{
	return vertexBufferSize;
}

int Mesh::getIndexBufferSize() const
{
	return indexBufferSize;
}
