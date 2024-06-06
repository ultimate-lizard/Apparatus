#include "UIContext.h"

#include <freetype/freetype.h>

#include "../Apparatus.h"
#include "../Rendering/Material.h"
#include "../Rendering/TextureArray.h"
#include "../Rendering/Sprite/Sprite.h"
#include "../Rendering/Sprite/TextBlock.h"
#include "../Core/AssetManager/Importer/TextureImporter.h"
#include "../Core/AssetManager/Importer/FontImporter.h"
#include "../Core/AssetManager/AssetManager.h"
#include "../Core/Input/InputHandler.h"
#include "Widget/NinePatchPanel.h"
#include "Widget/NinePatchButton.h"
#include "Widget/TextPanel.h"

UIContext::UIContext(InputHandler* inputHandler) :
	inputHandler(inputHandler)
{
	if (EventDispatcher* eventDispatcher = Apparatus::findEngineSystem<EventDispatcher>())
	{
		eventDispatcher->bind<WindowResizeEvent>(std::bind(&UIContext::onWindowResize, this, std::placeholders::_1));
	}
}

void UIContext::init()
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	if (!assetManager)
	{
		return;
	}

	if (Shader* spriteShader = assetManager->createAsset<Shader>("Shader_SpriteShader", "../Shaders/SpriteDefault.vert", "../Shaders/SpriteDefault.frag"))
	{
		if (Material* spriteMaterial = assetManager->createAsset<Material>("Material_Panel"))
		{
			spriteMaterial->setShader(spriteShader);
			spriteMaterial->createTextureParameter("spriteTexture", nullptr);
			spriteMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));
		}
	}

	if (Shader* ninePatchShader = assetManager->createAsset<Shader>("Shader_NinePatchShader", "../Shaders/SpriteDefault.vert", "../Shaders/NinePatchShader.frag"))
	{
		if (Material* ninePatchMaterial = assetManager->createAsset<Material>("Material_NinePatchPanel"))
		{
			ninePatchMaterial->setShader(ninePatchShader);
			ninePatchMaterial->createVec4Parameter("borders", glm::vec4(0.0f));
			ninePatchMaterial->createVec2Parameter("textureBorderSize", glm::vec4(0.0f));
			ninePatchMaterial->createVec2Parameter("spriteSize", glm::vec4(0.0f));
			ninePatchMaterial->createTextureParameter("spriteTexture", nullptr);
			ninePatchMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));
		}
	}

	if (Shader* textShader = assetManager->createAsset<Shader>("Shader_Text", "../Shaders/Text.vert", "../Shaders/Text.frag"))
	{
		if (Material* textMaterial = assetManager->createAsset<Material>("Material_TextPanel"))
		{
			textMaterial->setShader(textShader);
			textMaterial->createTextureArrayParameter("textTextureArray", nullptr);
			textMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));
		}
	}

	if (FontImporter* fontImporter = assetManager->getImporter<FontImporter>())
	{
		assetManager->createAsset("Font_Arial", fontImporter->import("../Fonts/Arial.ttf"));
	}
}

void UIContext::renderContext(SpriteRenderer* renderer)
{
	for (auto iter = spawnedWidgets.begin(); iter != spawnedWidgets.end(); ++iter)
	{
		if (Widget* widget = iter->get())
		{
			if (widget->isVisible())
			{
				widget->render(renderer);
			}
		}
	}
}

bool UIContext::handleKeyInput(InputKey key, KeyEventType type)
{
	if (!Apparatus::getWindow().isCursorVisible())
	{
		return false;
	}

	if (key == InputKey::MouseLeftButton)
	{
		for (auto& widget : spawnedWidgets)
		{
			if (widget)
			{
				if (widget->onKeyInput(key, type) && type != KeyEventType::Release && widget->isMouseCaptureEnabled())
				{
					return true;
				}
			}
		}
	}

	return false;
}

void UIContext::handleAxisInput(InputAxis axis, float value)
{
	if (!Apparatus::getWindow().isCursorVisible())
	{
		return;
	}

	if (axis == InputAxis::MouseX || axis == InputAxis::MouseY)
	{
		glm::ivec2 currentCursorPosition = Apparatus::getWindow().getMouseCursorPosition();
		for (auto& widget : spawnedWidgets)
		{
			if (widget)
			{
				widget->onMouseMove(currentCursorPosition);
			}
		}
	}
}

Button* UIContext::createNinePatchButton(const std::string& name, const std::string& idleTextureName, const std::string& hoverTextureName, const std::string& pressTextureName, unsigned int border, const std::string& labelText, unsigned int fontSize)
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	assert(assetManager);

	NinePatchButton* button = createWidget<NinePatchButton>(name);
	assert(button);

	button->setSizeToContentEnabled(true);

	button->setIdleTexture(assetManager->findAsset<Texture>(idleTextureName));
	button->setHoverTexture(assetManager->findAsset<Texture>(hoverTextureName));
	button->setPressTexture(assetManager->findAsset<Texture>(pressTextureName));

	// TODO: This should be found automatically?
	button->setSize({ 40, 32 });
	button->setBorder(border);

	if (!labelText.empty())
	{
		TextPanel* label = createWidget<TextPanel>(name + "TextPanel_Label");
		label->setText(labelText);
		label->setFontSize(fontSize);

		glm::ivec2 textSize = label->getGlobalSize();

		label->setMargin(Widget::Side::Top, textSize.y * 0.25f);
		label->setMargin(Widget::Side::Bottom, textSize.y * 0.75f);
		label->setMargin(Widget::Side::Left, 4);
		label->setMargin(Widget::Side::Right, 8);
		button->addChild(label);
	}

	return button;
}

void UIContext::onWindowResize(std::shared_ptr<WindowResizeEvent> event)
{
	invalidateTree();
}

void UIContext::refreshWidgetTree()
{
	for (auto& widget : spawnedWidgets)
	{
		if (widget)
		{
			if (!widget->getParent())
			{
				widget->refresh();
			}
		}
	}
}

void UIContext::invalidateTree()
{
	for (auto& widget : spawnedWidgets)
	{
		if (widget)
		{
			widget->invalidate();
		}
	}
}
