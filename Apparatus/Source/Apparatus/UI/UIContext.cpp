#include "UIContext.h"

#include <freetype/freetype.h>

#include "../Apparatus.h"
#include "../Rendering/Material.h"
#include "../Rendering/TextureArray.h"
#include "../Rendering/Sprite/Sprite.h"
#include "../Rendering/Sprite/TextBlock.h"
#include "../Core/TextureImporter.h"
#include "../Core/FontImporter.h"
#include "Widget/NinePatchPanel.h"

UIContext::UIContext()
{
	Apparatus::getEventDispatcher().bind<WindowResizeEvent>(std::bind(&UIContext::onWindowResize, this, std::placeholders::_1));
}

void UIContext::init()
{
	if (Shader* spriteShader = Apparatus::getAssetManager().createAsset<Shader>("Shader_SpriteShader", "../Shaders/SpriteDefault.vert", "../Shaders/SpriteDefault.frag"))
	{
		if (Material* spriteMaterial = Apparatus::getAssetManager().createAsset<Material>("Material_Panel"))
		{
			spriteMaterial->setShader(spriteShader);
			spriteMaterial->createTextureParameter("spriteTexture", nullptr);
			spriteMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));
		}
	}

	if (Shader* ninePatchShader = Apparatus::getAssetManager().createAsset<Shader>("Shader_NinePatchShader", "../Shaders/SpriteDefault.vert", "../Shaders/NinePatchShader.frag"))
	{
		if (Material* ninePatchMaterial = Apparatus::getAssetManager().createAsset<Material>("Material_NinePatchPanel"))
		{
			ninePatchMaterial->setShader(ninePatchShader);
			ninePatchMaterial->createVec4Parameter("borders", glm::vec4(0.0f));
			ninePatchMaterial->createVec2Parameter("textureBorderSize", glm::vec4(0.0f));
			ninePatchMaterial->createVec2Parameter("spriteSize", glm::vec4(0.0f));
			ninePatchMaterial->createTextureParameter("spriteTexture", nullptr);
			ninePatchMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));
		}
	}

	if (Shader* textShader = Apparatus::getAssetManager().createAsset<Shader>("Shader_Text", "../Shaders/Text.vert", "../Shaders/Text.frag"))
	{
		if (Material* textMaterial = Apparatus::getAssetManager().createAsset<Material>("Material_TextPanel"))
		{
			textMaterial->setShader(textShader);
			textMaterial->createTextureArrayParameter("textTextureArray", nullptr);
			textMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));
		}
	}

	if (FontImporter* fontImporter = Apparatus::getAssetManager().getImporter<FontImporter>())
	{
		Apparatus::getAssetManager().createAsset(fontImporter->import("Font_Arial", "../Fonts/Arial.ttf"));
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
	refreshWidgetTree();
}

void UIContext::refreshWidgetTree()
{
	for (auto& widget : spawnedWidgets)
	{
		if (widget)
		{
			// Find root widgets and start traversing from them
			if (!widget->getParent())
			{
				widget->refresh();
			}
		}
	}
}
