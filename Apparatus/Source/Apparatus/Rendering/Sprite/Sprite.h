#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "SpriteMesh.h"

class Material;
class Texture;

class Sprite
{
public:
	Sprite(Material* material, Texture* texture);
	virtual ~Sprite() = default;

	Sprite(const Sprite&) = delete;
	Sprite(Sprite&&) = delete;

	void setMaterial(Material* material);
	Material* getMaterial();

	void setTexture(Texture* texture);
	Texture* getTexture();

	void setTexturePosition(const glm::ivec2& coordinates);
	const glm::ivec2& getTexturePosition() const;

	void setTextureSize(const glm::ivec2& size);
	const glm::ivec2& getTextureSize() const;

	void setColor(const glm::vec4& color);
	const glm::vec4& getColor() const;

	void setPosition(const glm::ivec2& position);
	const glm::ivec2& getPosition() const;

	void setSize(const glm::ivec2& scale);
	const glm::ivec2& getSize() const;
	
	void setDepth(float depth);
	float getDepth() const;

	SpriteMesh* getSpriteMesh();

	void updateMesh();

private:
	Material* material;
	Texture* texture;

	glm::ivec2 position;
	glm::ivec2 size;
	float depth;

	glm::ivec2 texturePosition;
	glm::ivec2 textureBorderSize;

	glm::vec4 color;

	std::unique_ptr<SpriteMesh> spriteMesh;
};
