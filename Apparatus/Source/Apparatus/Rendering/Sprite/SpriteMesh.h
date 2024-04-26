#pragma once

#include <vector>

class SpriteMesh
{
public:
	SpriteMesh(size_t size);

	void setSubData(const std::vector<float>& vertices);
	void bind();

	const std::vector<float>& getVertices() const;

private:
	unsigned int vbo;
	unsigned int vao;

	std::vector<float> vertices;
};
