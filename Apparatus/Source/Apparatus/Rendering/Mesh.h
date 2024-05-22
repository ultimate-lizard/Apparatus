#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../Core/Asset.h"
#include "Sprite/VertexArrayObject.h"

struct aiMesh;
struct aiNode;

struct Vertex
{
	glm::vec3 position{};
	glm::vec2 uv{};
	glm::vec3 normal{};
	glm::vec4 color{};
};

class Mesh : public Asset
{
public:
	Mesh(const std::string& resourceName, size_t vertexBufferSize, size_t indexBufferSize);
	Mesh(const std::string& resourceName, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int materialIndex);
	~Mesh();

	Mesh() = delete;
	Mesh(Mesh&&) = delete;
	Mesh(const Mesh&) = delete;
	void operator=(const Mesh&) = delete;

	virtual void init() override;

	void bind() const;

	void setSubData(const std::vector<Vertex>& vertices);
	void setSubData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	const std::vector<Vertex>& getVertices() const;
	const std::vector<unsigned int>& getIndices() const;

	unsigned int getMaterialIndex() const;
	size_t getVertexBufferSize() const;
	size_t getIndexBufferSize() const;

private:
	VertexArrayObject vao;
	unsigned int vbo;
	unsigned int ebo;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int materialIndex;

	// How much data in bytes will be allocated for EBO and VBO for dynamic draw if no vertice and index data provided during creation of the mesh
	size_t vertexBufferSize;
	size_t indexBufferSize;

	friend class ModelImporter;
};
