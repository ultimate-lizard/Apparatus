#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "../Core/Asset.h"
#include "Sprite/VertexArrayObject.h"

struct aiMesh;
struct aiNode;

struct ModelVertex
{
	glm::vec3 position{};
	glm::vec2 uv{};
	glm::vec3 normal{};
	glm::vec4 color{};
};

struct VertexBufferInterface
{
	virtual void* getData() = 0;
	virtual int getSize() const = 0;
};

template <typename VertexType>
struct VertexBuffer : public VertexBufferInterface
{
	virtual void* getData() override
	{
		return vertices.data();
	}

	virtual int getSize() const override
	{
		return static_cast<int>(vertices.size()) * sizeof(VertexType);
	}

	std::vector<VertexType> vertices;
};

class Mesh : public Asset
{
public:
	Mesh(int vertexBufferSize, int indexBufferSize);
	Mesh(std::shared_ptr<VertexBufferInterface> vertices, const std::vector<unsigned int>& indices, unsigned int materialIndex);
	~Mesh();

	Mesh() = delete;
	Mesh(Mesh&&) = delete;
	Mesh(const Mesh&) = delete;
	void operator=(const Mesh&) = delete;

	virtual void init() override;

	void bind() const;

	void setSubData(std::shared_ptr<VertexBufferInterface> vertices);
	void setSubData(std::shared_ptr<VertexBufferInterface> vertices, const std::vector<unsigned int>& indices);

	template <typename BufferType>
	std::shared_ptr<BufferType> getVertexBuffer() const;

	const std::vector<unsigned int>& getIndices() const;

	unsigned int getMaterialIndex() const;

	int getVertexBufferSize() const;
	int getIndexBufferSize() const;

private:
	VertexArrayObject vao;
	unsigned int vbo;
	unsigned int ebo;

	std::shared_ptr<VertexBufferInterface> vertexBuffer;
	std::vector<unsigned int> indices;
	unsigned int materialIndex;

	int vertexBufferSize;
	int indexBufferSize;

	friend class ModelImporter;
};

template<typename BufferType>
inline std::shared_ptr<BufferType> Mesh::getVertexBuffer() const
{
	return std::dynamic_pointer_cast<BufferType>(vertexBuffer);
}
