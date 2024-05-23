#include "Mesh.h"

#include <glad/glad.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Logger.h"

Mesh::Mesh(int vertexBufferSize, int indexBufferSize) :
	vbo(0),
	ebo(0),
	materialIndex(0),
	vertexBufferSize(vertexBufferSize),
	indexBufferSize(indexBufferSize)
{

}

Mesh::Mesh(std::shared_ptr<VertexBufferInterface> vertices, const std::vector<unsigned int>& indices, unsigned int materialIndex) :
	vbo(0),
	ebo(0),
	vertexBuffer(vertices),
	indices(indices),
	materialIndex(materialIndex),
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

	if (vertexBuffer)
	{
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer->getSize(), vertexBuffer->getData(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	vao.bind();
	vao.setStride(sizeof(ModelVertex));
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

void Mesh::setSubData(std::shared_ptr<VertexBufferInterface> vertices)
{
	bind();

	vertexBuffer = vertices;

	if (vertexBuffer)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer->getSize(), vertexBuffer->getData());
	}
}

void Mesh::setSubData(std::shared_ptr<VertexBufferInterface> vertices, const std::vector<unsigned int>& indices)
{
	bind();

	vertexBuffer = vertices;
	this->indices = indices;

	if (vertexBuffer)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer->getSize(), vertexBuffer->getData());
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
