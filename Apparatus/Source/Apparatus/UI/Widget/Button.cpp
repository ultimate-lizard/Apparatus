#include "Button.h"

#include "../../Core/Logger.h"
#include "ImagePanel.h"

Button::Button() :
	idleStatePanel(nullptr),
	hoverStatePanel(nullptr),
	pressStatePanel(nullptr),
	currentStatePanel(nullptr),
	buttonState(ButtonState::Idle)
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
	if (currentStatePanel)
	{
		return currentStatePanel->getSize();
	}

	return {};
}

void Button::refresh()
{
	Widget::refresh();

	if (invalidated)
	{
		// TODO: Make them invisible
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

		// TODO: Make it visible
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

	invalidate();
}

void Button::onMouseLeave()
{
	if (buttonState == Button::ButtonState::Hover)
	{
		setButtonState(Button::ButtonState::Idle);
	}

	invalidate();
}

void Button::onKeyInput(InputKey key, KeyEventType type)
{
	if (key == InputKey::MouseLeftButton)
	{
		if (type == KeyEventType::Press)
		{
			setButtonState(Button::ButtonState::Press);
		}
		else
		{
			setButtonState(Button::ButtonState::Idle);
		}
	}

	LOG("The buttom has been pressed!", LogLevel::Info);

	invalidate();
}
