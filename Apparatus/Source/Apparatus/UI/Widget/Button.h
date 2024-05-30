#pragma once

#include "BoxModelPanel.h"

class ImagePanel;
class TextPanel;

class Button : public BoxModelPanel
{
public:
	enum class ButtonState
	{
		Idle,
		Hover,
		Press
	};

	Button();

	void addPanelForState(ImagePanel* idleState, ButtonState state);
	void addLabel(TextPanel* textPanel);
	void setButtonState(ButtonState state);

	virtual glm::ivec2 getGlobalSize() const override;
	virtual void refresh() override;

	virtual void onMouseEnter() override;
	virtual void onMouseLeave() override;
	virtual bool onKeyInput(InputKey key, KeyEventType type) override;

	void setLabelClickOffset(int offset);

private:
	virtual void addChild(Widget* child) override;

	ImagePanel* idleStatePanel;
	ImagePanel* hoverStatePanel;
	ImagePanel* pressStatePanel;
	ImagePanel* currentStatePanel;

	TextPanel* label;
	int labelClickOffset;
	glm::ivec2 labelPosition;

	ButtonState buttonState;

	bool hovered;
};
