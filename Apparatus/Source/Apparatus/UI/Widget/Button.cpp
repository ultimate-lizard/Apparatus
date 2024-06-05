#include "Button.h"

#include "../../Core/Logger.h"
#include "ImagePanel.h"
#include "TextPanel.h"
#include "../../Apparatus.h"
#include "../../Rendering/Sprite/Sprite.h"
#include "../../Rendering/Material.h"
#include "../../Core/AssetManager/AssetManager.h"

Button::Button() :
	//idleStatePanel(nullptr),
	//hoverStatePanel(nullptr),
	//pressStatePanel(nullptr),
	//currentStatePanel(nullptr),
	//label(nullptr),
	currentSprite(nullptr),
	buttonState(ButtonState::Idle),
	hovered(false),
	childClickOffset(2)
{
	mouseCaptureEnabled = true;
}

void Button::init()
{
	Widget::init();

	idleSprite = std::make_unique<Sprite>();
	hoverSprite = std::make_unique<Sprite>();
	pressSprite = std::make_unique<Sprite>();

	currentSprite = idleSprite.get();

	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	Material* buttonMaterial = assetManager->findAsset<Material>("Material_Panel");
	idleSprite->setMaterial(buttonMaterial);
	hoverSprite->setMaterial(buttonMaterial);
	pressSprite->setMaterial(buttonMaterial);

	//invalidate();
}

//void Button::addPanelForState(ImagePanel* panel, ButtonState state)
//{
//	if (panel)
//	{
//		switch (state)
//		{
//		case Button::ButtonState::Idle:
//			idleStatePanel = panel;
//			break;
//		case Button::ButtonState::Hover:
//			hoverStatePanel = panel;
//			break;
//		case Button::ButtonState::Press:
//			pressStatePanel = panel;
//			break;
//		}
//
//		addChild(panel);
//
//		invalidate();
//	}
//}
//
//void Button::addLabel(TextPanel* textPanel)
//{
//	if (textPanel)
//	{
//		this->label = textPanel;
//
//		addChild(textPanel);
//
//		invalidate();
//	}
//}

void Button::setButtonState(ButtonState state)
{
	this->buttonState = state;

	invalidate();
}

glm::ivec2 Button::getGlobalSize() const
{
	return size;
}

void Button::refresh()
{
	Widget::refresh();

	if (invalidated)
	{
		switch (buttonState)
		{
		case Button::ButtonState::Idle:
			currentSprite = idleSprite.get();
			break;
		case Button::ButtonState::Hover:
			currentSprite = hoverSprite.get();
			break;
		case Button::ButtonState::Press:
			currentSprite = pressSprite.get();
			break;
		}

		if (!currentSprite && idleSprite)
		{
			currentSprite = pressSprite.get();
		}

		for (Widget* child : children)
		{
			if (buttonState == Button::ButtonState::Press)
			{
				child->setPosition(glm::ivec2(childClickOffset));
			}
			else
			{
				child->setPosition(glm::ivec2(0));
			}
		}

		if (currentSprite)
		{
			currentSprite->setPosition(getGlobalPosition());
			auto globalSize = getGlobalSize();
			currentSprite->setSize(globalSize);
			currentSprite->setTextureSize(globalSize);
			currentSprite->rebuildMesh();
		}

		invalidated = false;
	}
}

void Button::render(SpriteRenderer* renderer)
{
	renderer->push(currentSprite);
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
		}
		else
		{
			setButtonState(hovered ? Button::ButtonState::Hover : Button::ButtonState::Idle);
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

void Button::setIdleTexture(Texture* texture)
{
	idleSprite->setTexture(texture);
	invalidate();
}

void Button::setHoverTexture(Texture* texture)
{
	hoverSprite->setTexture(texture);
	invalidate();
}

void Button::setPressTexture(Texture* texture)
{
	pressSprite->setTexture(texture);
	invalidate();
}

void Button::setLabelClickOffset(int offset)
{
	this->childClickOffset = offset;
}
