#include "Button.h"

#include "../../Core/Logger.h"
#include "ImagePanel.h"

Button::Button() :
	idleStatePanel(nullptr),
	hoverStatePanel(nullptr),
	pressStatePanel(nullptr),
	currentStatePanel(nullptr),
	buttonState(ButtonState::Idle),
	hovered(false)
{
	mouseCaptureEnabled = true;
}

void Button::setPanelForState(ImagePanel* panel, ButtonState state)
{
	if (panel)
	{
		switch (state)
		{
		case Button::ButtonState::Idle:
			idleStatePanel = panel;
			break;
		case Button::ButtonState::Hover:
			hoverStatePanel = panel;
			break;
		case Button::ButtonState::Press:
			pressStatePanel = panel;
			break;
		}

		addChild(panel);

		invalidate();
	}
}

void Button::setButtonState(ButtonState state)
{
	this->buttonState = state;

	invalidate();
}

glm::ivec2 Button::getGlobalSize() const
{
	if (idleStatePanel)
	{
		return idleStatePanel->getSize();
	}

	return {};
}

void Button::refresh()
{
	Widget::refresh();

	if (invalidated)
	{
		if (idleStatePanel)
		{
			idleStatePanel->setVisibility(false);
		}

		if (hoverStatePanel)
		{
			hoverStatePanel->setVisibility(false);
		}

		if (pressStatePanel)
		{
			pressStatePanel->setVisibility(false);
		}

		if (currentStatePanel)
		{
			currentStatePanel->setVisibility(false);
		}

		switch (buttonState)
		{
		case Button::ButtonState::Idle:
			currentStatePanel = idleStatePanel;
			break;
		case Button::ButtonState::Hover:
			currentStatePanel = hoverStatePanel;
			break;
		case Button::ButtonState::Press:
			currentStatePanel = pressStatePanel;
			break;
		}

		if (currentStatePanel)
		{
			currentStatePanel->setVisibility(true);
		}

		invalidated = false;
	}
}

void Button::onMouseEnter()
{
	if (buttonState != Button::ButtonState::Press)
	{
		setButtonState(Button::ButtonState::Hover);
	}

	hovered = true;

	invalidate();
}

void Button::onMouseLeave()
{
	if (buttonState == Button::ButtonState::Hover)
	{
		setButtonState(Button::ButtonState::Idle);
	}

	hovered = false;

	invalidate();
}

bool Button::onKeyInput(InputKey key, KeyEventType type)
{
	if (key == InputKey::MouseLeftButton)
	{
		if (type == KeyEventType::Press && hovered)
		{
			setButtonState(Button::ButtonState::Press);
		}
		else
		{
			setButtonState(hovered ? Button::ButtonState::Hover : Button::ButtonState::Idle);
		}

		invalidate();

		return true;
	}

	return false;
}
