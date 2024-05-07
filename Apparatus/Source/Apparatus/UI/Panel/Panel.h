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

	void setPosition(const glm::vec2& position);
	const glm::vec2& getPosition() const;

	void setSize(const glm::vec2& size);
	const glm::vec2& getSize() const;

	void setTexturePosition(const glm::vec2& texturePosition);
	glm::vec2 getTexturePosition() const;

	void setTextureSize(const glm::vec2& textureSize);
	glm::vec2 getTextureSize() const;

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
	glm::vec2 calculateSpritePosition() const;
	glm::vec2 calculateSpriteSize() const;

	Sprite sprite;

	std::vector<Panel*> children;
	Panel* parent;

	glm::vec2 position;
	glm::vec2 size;

	Alignment horizontalAlignment;
	Alignment verticalAlignment;

	std::array<int, 4> margins;
	std::array<int, 4> paddings;
};
