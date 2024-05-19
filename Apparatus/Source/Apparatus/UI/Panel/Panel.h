#pragma once

#include <vector>
#include <array>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "../../Rendering/Sprite/Sprite.h"

class Material;
class SpriteRenderer;
class TextBlock;
class UIContext;

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

	virtual void init() {}
	virtual void refresh();

	virtual glm::ivec2 getGlobalPosition() const;
	virtual glm::ivec2 getGlobalSize() const = 0;

	virtual void render(SpriteRenderer* renderer) {}

	void setName(const std::string& name);
	const std::string& getName() const;

	Widget* getParent();
	virtual void addChild(Widget* child);
	Widget* getChild(size_t index) const;
	size_t getChildrenCount() const;

	void setPosition(const glm::ivec2& position);
	const glm::ivec2& getPosition() const;

	void setHorizontalAlignment(Alignment alignment);
	void setVerticalAlignment(Alignment alignment);

protected:
	std::string name;

	std::vector<Widget*> children;
	Widget* parent;

	glm::ivec2 position;

	Alignment horizontalAlignment;
	Alignment verticalAlignment;
};

class BoxModelPanel : public Widget
{
public:
	BoxModelPanel();

	virtual glm::ivec2 getGlobalPosition() const override;
	virtual glm::ivec2 getGlobalSize() const override;

	void setSize(const glm::ivec2& size);
	const glm::ivec2& getSize() const;

	void setMargin(Side side, int margin);
	int getMargin(Side side) const;

protected:
	glm::ivec2 size;
	std::array<int, 4> margins;
};

class ImagePanel : public BoxModelPanel
{
public:
	virtual void init() override;
	virtual void refresh() override;

	virtual void render(SpriteRenderer* renderer) override;

	Sprite* getSprite();
	void setTexture(Texture* texture);

protected:
	std::unique_ptr<Sprite> sprite;
};

class TextPanel : public BoxModelPanel
{
public:
	virtual void init() override;
	virtual void refresh() override;

	virtual void render(SpriteRenderer* renderer) override;

	TextBlock* getTextBlock();
	void setText(const std::string& text);
	void setColor(const glm::vec4& color);
	void setFontSize(float fontSize);

	void setDepth(float depth);

protected:
	std::unique_ptr<TextBlock> textBlock;
};

class HorizontalPanel : public Widget
{
public:
	virtual void addChild(Widget* child) override;

	virtual glm::ivec2 getGlobalSize() const override;
	virtual void refresh() override;

protected:
	std::vector<std::unique_ptr<BoxModelPanel>> childContainers;
};
