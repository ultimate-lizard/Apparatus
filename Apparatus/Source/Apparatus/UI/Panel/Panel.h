#pragma once

#include <vector>
#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "../../Rendering/Sprite/Sprite.h"

class Material;
class SpriteRenderer;

class Panel
{
public:
	enum class Side
	{
		Top = 0,
		Bottom,
		Left,
		Right
	};

	// Vertical left alignment is actually up, and so on...
	enum class Alignment
	{
		Left,
		Center,
		Right,
		Fill
	};

	Panel(Material* material, Texture* texture);

	Panel* getParent();
	void addChild(Panel* child);

	void setTexture(Texture* texture);
	Texture* getTexture();

	void setMaterial(Material* material);
	Material* getMaterial();

	void setPosition(const glm::ivec2& position);
	const glm::ivec2& getPosition() const;

	void setSize(const glm::ivec2& size);
	const glm::ivec2& getSize() const;

	void setTexturePosition(const glm::ivec2& texturePosition);
	glm::ivec2 getTexturePosition() const;

	void setTextureSize(const glm::ivec2& textureSize);
	glm::ivec2 getTextureSize() const;

	void setDepth(float depth);
	float getDepth() const;

	void setColor(const glm::vec4& color);
	glm::vec4 getColor() const;

	void setHorizontalAlignment(Alignment alignment);
	void setVerticalAlignment(Alignment alignment);

	void setMargin(Side side, int margin);
	int getMargin(Side side) const;

	void setPadding(Side side, int padding);
	int getPadding(Side side) const;

	void pushToRenderer(SpriteRenderer* renderer);
	virtual void refresh();

protected:
	glm::ivec2 calculateSpritePosition() const;
	glm::ivec2 calculateSpriteSize() const;

	Sprite sprite;

	std::vector<Panel*> children;
	Panel* parent;

	glm::ivec2 position;
	glm::ivec2 size;

	Alignment horizontalAlignment;
	Alignment verticalAlignment;

	std::array<int, 4> margins;
	std::array<int, 4> paddings;
};
