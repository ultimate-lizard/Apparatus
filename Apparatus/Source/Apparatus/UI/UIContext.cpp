#include "UIContext.h"

#include <freetype/freetype.h>

#include "../Apparatus.h"
#include "../Rendering/Material.h"
#include "../Rendering/TextureArray.h"
#include "../Rendering/Sprite/Sprite.h"
#include "../Rendering/Sprite/TextBlock.h"
#include "../Core/TextureImporter.h"
#include "../Core/FontImporter.h"
#include "../Core/AssetManager.h"
#include "Widget/NinePatchPanel.h"

UIContext::UIContext()
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
			widget->render(renderer);
		}
	}
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
