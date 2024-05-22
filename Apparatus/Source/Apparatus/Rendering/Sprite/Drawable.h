#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "VertexArrayObject.h"

class Material;

class Drawable
{
public:
	Drawable(int bufferSize);
	virtual ~Drawable() = default;
	Drawable(const Drawable&) = delete;
	Drawable(Drawable&&) = delete;

	virtual void rebuildMesh() = 0;

	void setPosition(const glm::ivec2& position);
	const glm::ivec2& getPosition() const;

	void setSize(const glm::ivec2& size);
	const glm::ivec2& getSize() const;

	void setDepth(float depth);
	float getDepth() const;

	void setMaterial(Material* material);
	Material* getMaterial();

	void setColor(const glm::vec4& color);
	const glm::vec4& getColor() const;

	SpriteMesh* getSpriteMesh();

protected:
	Material* material;

	glm::ivec2 position;
	glm::ivec2 size;
	float depth;

	glm::vec4 color;

	std::unique_ptr<SpriteMesh> spriteMesh;
};
