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
	childClickOffset(2),
	executedOnReleaseEnabled(false)
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

	initMaterial();
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

//glm::ivec2 Button::getGlobalSize() const
//{
//	if (sizeToContent)
//	{
//		glm::ivec2 maxSize(0);
//
//		for (Widget* child : children)
//		{
//			const glm::ivec2 childSize = child->getGlobalSize();
//			if (glm::all(glm::greaterThan(childSize, maxSize)))
//			{
//				maxSize = childSize;
//			}
//		}
//
//		return maxSize;
//	}
//
//	return size;
//}

bool Button::refresh()
{
	bool wasInvalidated = Widget::refresh();

	if (wasInvalidated)
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
				//child->setPosition(glm::ivec2(0));
			}
		}

		refreshSprite();

		invalidated = false;
	}

	return wasInvalidated;
}

void Button::render(SpriteRenderer* renderer)
{
	if (renderer && isVisible())
	{
		renderer->push(currentSprite);
	}
}

void Button::onMouseEnter()
{
	if (!isVisible())
	{
		return;
	}

	if (buttonState != Button::ButtonState::Press)
	{
		setButtonState(Button::ButtonState::Hover);
	}

	hovered = true;

	invalidate();
}

void Button::onMouseLeave()
{
	if (!isVisible())
	{
		return;
	}

	if (buttonState == Button::ButtonState::Hover)
	{
		setButtonState(Button::ButtonState::Idle);
	}

	hovered = false;

	invalidate();
}

bool Button::onKeyInput(InputKey key, KeyEventType type)
{
	if (isExecutedOnReleaseEnabled() && key == InputKey::MouseLeftButton && type == KeyEventType::Release && buttonState == ButtonState::Press && hovered)
	{
		execute();
	}

	if (!Widget::onKeyInput(key, type))
	{
		return false;
	}

	if (key == InputKey::MouseLeftButton)
	{
		if (type == KeyEventType::Press && hovered)
		{
			setButtonState(Button::ButtonState::Press);
			if (!isExecutedOnReleaseEnabled())
			{
				execute();
			}
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

void Button::setCallback(std::function<void()> callback)
{
	this->callback = callback;
}

void Button::execute()
{
	if (callback)
	{
		callback();
	}
}

void Button::setExecutedOnReleaseEnabled(bool enabled)
{
	this->executedOnReleaseEnabled = enabled;
}

bool Button::isExecutedOnReleaseEnabled() const
{
	return executedOnReleaseEnabled == true;
}

void Button::initMaterial()
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	Material* buttonMaterial = assetManager->findAsset<Material>("Material_Panel");
	idleSprite->setMaterial(buttonMaterial);
	hoverSprite->setMaterial(buttonMaterial);
	pressSprite->setMaterial(buttonMaterial);
}

void Button::refreshSprite()
{
	if (!currentSprite)
	{
		return;
	}

	currentSprite->setPosition(getGlobalPosition());
	const glm::ivec2 globalSize = getGlobalSize();
	currentSprite->setSize(globalSize);
	currentSprite->setTextureSize(globalSize);
	currentSprite->rebuildMesh();
}
