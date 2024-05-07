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

	void setTexturePosition(const glm::vec2& coordinates);
	const glm::vec2& getTexturePosition() const;

	void setTextureSize(const glm::vec2& size);
	const glm::vec2& getTextureSize() const;

	void setColor(const glm::vec4& color);
	const glm::vec4& getColor() const;

	void setPosition(const glm::vec2& position);
	const glm::vec2& getPosition() const;

	void setSize(const glm::vec2& scale);
	const glm::vec2& getSize() const;
	
	void setDepth(float depth);
	float getDepth() const;

	SpriteMesh* getSpriteMesh();

	void updateMesh();

private:
	Material* material;
	Texture* texture;

	glm::vec2 position;
	glm::vec2 size;
	float depth;

	glm::vec2 texturePosition;
	glm::vec2 textureBorderSize;

	glm::vec4 color;

	std::unique_ptr<SpriteMesh> spriteMesh;
};
