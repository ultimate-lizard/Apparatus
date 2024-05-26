#pragma once

#include <vector>
#include <memory>
#include <string>

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

	void invalidate();

	void setMouseCaptureEnabled(bool enabled);
	bool isMouseCaptureEnabled() const;

	virtual void onKeyInput(InputKey key, KeyEventType type) {}

protected:
	std::string name;

	std::vector<Widget*> children;
	Widget* parent;

	glm::ivec2 position;

	Alignment horizontalAlignment;
	Alignment verticalAlignment;

	bool invalidated;
	
	bool mouseCaptureEnabled;
};
