#pragma once

#include "Drawable.h"

class Texture;

class Sprite : public Drawable
{
public:
	Sprite();
	virtual ~Sprite() = default;
	Sprite(const Sprite&) = delete;
	Sprite(Sprite&&) = delete;

	// TODO: Do it only when dirty
	virtual void rebuildMesh() override;

	void setTexture(Texture* texture);
	Texture* getTexture();

	void setTexturePosition(const glm::ivec2& coordinates);
	const glm::ivec2& getTexturePosition() const;

	void setTextureSize(const glm::ivec2& size);
	const glm::ivec2& getTextureSize() const;

private:
	Texture* texture;

	glm::ivec2 texturePosition;
	glm::ivec2 textureSize;
};
