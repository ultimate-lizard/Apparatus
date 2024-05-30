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
#include "Widget/Button.h"
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

Button* UIContext::createNinePatchButton(const std::string& name, const std::string& idleTextureName, const std::string& hoverTextureName, const std::string& pressTextureName, unsigned int border, const std::string& label, unsigned int fontSize)
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	assert(assetManager);

	Button* button = createWidget<Button>(name);
	assert(button);
	// TODO: This should be found automatically?
	button->setSize({ 40, 32 });

	NinePatchPanel* idlePanel = createWidget<NinePatchPanel>(name + "NinePatchPanel_Idle");
	assert(idlePanel);
	idlePanel->setTexture(assetManager->findAsset<Texture>(idleTextureName));
	idlePanel->setBorder(border);

	NinePatchPanel* hoverPanel = createWidget<NinePatchPanel>(name + "NinePatchPanel_Hover");
	assert(hoverPanel);
	hoverPanel->setTexture(assetManager->findAsset<Texture>(hoverTextureName));
	hoverPanel->setBorder(border);

	NinePatchPanel* pressPanel = createWidget<NinePatchPanel>(name + "NinePatchPanel_Press");
	pressPanel->setTexture(assetManager->findAsset<Texture>(pressTextureName));
	pressPanel->setBorder(border);

	if (!label.empty())
	{
		TextPanel* fileLabel = createWidget<TextPanel>(name + "TextPanel_Label");
		fileLabel->setText(label);
		fileLabel->setFontSize(fontSize);
		// fileLabel->setHorizontalAlignment(Widget::Alignment::Center);
		// fileLabel->setVerticalAlignment(Widget::Alignment::Center);

		button->addLabel(fileLabel);
	}

	button->addPanelForState(idlePanel, Button::ButtonState::Idle);
	button->addPanelForState(hoverPanel, Button::ButtonState::Hover);
	button->addPanelForState(pressPanel, Button::ButtonState::Press);

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
