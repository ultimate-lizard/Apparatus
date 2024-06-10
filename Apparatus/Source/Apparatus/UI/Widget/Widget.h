#pragma once

#include <vector>
#include <memory>
#include <string>
#include <array>

#include <glm/glm.hpp>

#include "../../Core/Input/InputHandler.h"

class SpriteRenderer;

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
	virtual bool refresh();

	virtual glm::ivec2 getGlobalPosition();
	virtual glm::ivec2 getGlobalSize();

	void setSize(const glm::ivec2& size);
	virtual const glm::ivec2& getSize() const;

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

	void setMargin(Widget::Side side, int margin);
	void setMargin(int margin);
	int getMargin(Widget::Side side) const;

	void setSizeToContentEnabled(bool enabled);
	bool isSizeToContentEnabled() const;

	void invalidate();

	void setMouseCaptureEnabled(bool enabled);
	bool isMouseCaptureEnabled() const;

	void setVisibility(bool visible);
	bool isVisible() const;

	virtual void onMouseMove(const glm::ivec2& cursorPosition);

	virtual void onMouseEnter() {}
	virtual void onMouseLeave() {}
	virtual bool onKeyInput(InputKey key, KeyEventType type);

	bool isContaining(const glm::ivec2& coordiante);

protected:
	std::string name;

	std::vector<Widget*> children;
	Widget* parent;

	glm::ivec2 position;
	glm::ivec2 size;

	Alignment horizontalAlignment;
	Alignment verticalAlignment;

	std::array<int, 4> margins;

	bool sizeToContent;
	glm::ivec2 contentSize;

	bool invalidated;
	
	bool mouseCaptureEnabled;
	bool visible;
	bool containsCursor;
};
