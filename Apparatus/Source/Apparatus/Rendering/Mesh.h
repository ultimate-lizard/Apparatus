#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../Core/Resource.h"

struct aiMesh;
struct aiNode;

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec4 color;
};

class Mesh : public Resource
{
public:
	Mesh(const std::string& resourceName, long long bufferSize);
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

private:
	glm::mat4 processTransform(aiNode* node);

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int materialIndex;
	long long bufferSize;
};
