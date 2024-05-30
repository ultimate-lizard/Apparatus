#include "Button.h"

#include "../../Core/Logger.h"
#include "ImagePanel.h"
#include "TextPanel.h"
#include "../../Apparatus.h"

Button::Button() :
	idleStatePanel(nullptr),
	hoverStatePanel(nullptr),
	pressStatePanel(nullptr),
	currentStatePanel(nullptr),
	label(nullptr),
	labelClickOffset(2),
	labelPosition(0),
	buttonState(ButtonState::Idle),
	hovered(false)
{
	mouseCaptureEnabled = true;
}

void Button::addPanelForState(ImagePanel* panel, ButtonState state)
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

void Button::addLabel(TextPanel* textPanel)
{
	if (textPanel)
	{
		this->label = textPanel;

		addChild(textPanel);

		labelPosition = textPanel->getPosition();

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
		if (idleStatePanel)
		{
			idleStatePanel->setVisibility(false);
		}

		if (hoverStatePanel)
		{
			hoverStatePanel->setVisibility(false);
		}
		else
		{
			hoverStatePanel = idleStatePanel;
		}

		if (pressStatePanel)
		{
			pressStatePanel->setVisibility(false);
		}
		else
		{
			pressStatePanel = idleStatePanel;
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
	const glm::ivec2 cursorPosition = Apparatus::getWindow().getMouseCursorPosition();
	if (!isContaining(cursorPosition))
	{
		return false;
	}

	if (key == InputKey::MouseLeftButton)
	{
		if (type == KeyEventType::Press && hovered)
		{
			setButtonState(Button::ButtonState::Press);

			if (label)
			{
				label->setPosition(labelPosition + labelClickOffset);
			}
		}
		else
		{
			setButtonState(hovered ? Button::ButtonState::Hover : Button::ButtonState::Idle);

			if (label)
			{
				label->setPosition(labelPosition);
			}
		}

		invalidate();

		if (type == KeyEventType::Press)
		{
			LOG("Click!", LogLevel::Trace);
		}

		return true;
	}

	return false;
}

void Button::setLabelClickOffset(int offset)
{
	this->labelClickOffset = offset;
}

void Button::addChild(Widget* child)
{
	Widget::addChild(child);
}
