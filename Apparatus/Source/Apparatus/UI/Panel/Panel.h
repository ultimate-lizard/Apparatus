#pragma once

#include <vector>
#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "../../Rendering/Sprite/Sprite.h"

class Material;
class SpriteRenderer;
class TextBlock;

class Widget
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

	Widget();
	virtual ~Widget() = default;

	virtual void refresh();

	virtual glm::ivec2 getGlobalPosition() const;
	virtual glm::ivec2 getGlobalSize() const = 0;

	Widget* getParent();
	virtual void addChild(Widget* child);
	Widget* getChild(size_t index) const;
	size_t getChildrenCount() const;

	void setPosition(const glm::ivec2& position);
	const glm::ivec2& getPosition() const;

	void setHorizontalAlignment(Alignment alignment);
	void setVerticalAlignment(Alignment alignment);

protected:
	std::vector<Widget*> children;
	Widget* parent;

	glm::ivec2 position;

	Alignment horizontalAlignment;
	Alignment verticalAlignment;

	std::array<int, 4> margins;
};

class BoxModelWidget : public Widget
{
public:
	virtual glm::ivec2 getGlobalPosition() const override;
	virtual glm::ivec2 getGlobalSize() const override;

	void setSize(const glm::ivec2& size);
	const glm::ivec2& getSize() const;

	void setMargin(Side side, int margin);
	int getMargin(Side side) const;

protected:
	glm::ivec2 size;
};

class Panel : public BoxModelWidget
{
public:
	Panel();

	void setSprite(Sprite* sprite);
	Sprite* getSprite();

	virtual void refresh() override;

protected:
	Sprite* sprite;
};

class TextPanel : public BoxModelWidget
{
public:
	void setTextBlock(TextBlock* textBlock);
	TextBlock* getTextBlock();

	virtual void refresh() override;

protected:
	TextBlock* textBlock;
};

class HorizontalPanel : public Widget
{
public:
	virtual void addChild(Widget* child) override;

	virtual glm::ivec2 getGlobalSize() const override;
	virtual void refresh() override;

protected:
	std::vector<std::unique_ptr<BoxModelWidget>> childContainers;
};
