#pragma once

#include "BoxModelPanel.h"

class ImagePanel;

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

	void setPanelForState(ImagePanel* idleState, ButtonState state);
	void setButtonState(ButtonState state);

	virtual glm::ivec2 getGlobalSize() const override;
	virtual void refresh() override;

	virtual void onMouseEnter() override;
	virtual void onMouseLeave() override;
	virtual bool onKeyInput(InputKey key, KeyEventType type) override;

private:
	ImagePanel* idleStatePanel;
	ImagePanel* hoverStatePanel;
	ImagePanel* pressStatePanel;
	ImagePanel* currentStatePanel;

	ButtonState buttonState;

	bool hovered;
};
