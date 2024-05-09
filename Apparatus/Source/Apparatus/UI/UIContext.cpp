#include "UIContext.h"

#include <freetype/freetype.h>
#include FT_FREETYPE_H  

#include "../Apparatus.h"
#include "../Rendering/Material.h"
#include "../Rendering/TextureArray.h"
#include "../Rendering/Sprite/Sprite.h"
#include "../Rendering/Sprite/TextBlock.h"
#include "../Core/TextureImporter.h"
#include "Panel/NinePatchPanel.h"

UIContext::UIContext() :
	root(nullptr)
{
	Apparatus::getEventDispatcher().bind<WindowResizeEvent>(std::bind(&UIContext::onWindowResize, this, std::placeholders::_1));
}

void UIContext::init()
{
	if (Shader* spriteShader = Apparatus::getAssetManager().createAsset<Shader>("Shader_SpriteShader", "../Shaders/SpriteDefault.vert", "../Shaders/SpriteDefault.frag"))
	{
		if (Material* spriteMaterial = Apparatus::getAssetManager().createAsset<Material>("Material_Sprite"))
		{
			spriteMaterial->setShader(spriteShader);
			spriteMaterial->createTextureParameter("spriteTexture", nullptr);
			spriteMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));

			materialsMap.emplace(typeid(Panel), spriteMaterial);
		}
	}

	if (Shader* ninePatchShader = Apparatus::getAssetManager().createAsset<Shader>("Shader_NinePatchShader", "../Shaders/SpriteDefault.vert", "../Shaders/NinePatchShader.frag"))
	{
		if (Material* ninePatchMaterial = Apparatus::getAssetManager().createAsset<Material>("Material_NinePatch"))
		{
			ninePatchMaterial->setShader(ninePatchShader);
			ninePatchMaterial->createVec4Parameter("borders", glm::vec4(0.0f));
			ninePatchMaterial->createVec2Parameter("textureBorderSize", glm::vec4(0.0f));
			ninePatchMaterial->createVec2Parameter("spriteSize", glm::vec4(0.0f));
			ninePatchMaterial->createTextureParameter("spriteTexture", nullptr);
			ninePatchMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));

			materialsMap.emplace(typeid(NinePatchPanel), ninePatchMaterial);
		}
	}

	if (Shader* textShader = Apparatus::getAssetManager().createAsset<Shader>("Shader_Text", "../Shaders/SpriteDefault.vert", "../Shaders/Text.frag"))
	{
		if (Material* textMaterial = Apparatus::getAssetManager().createAsset<Material>("Material_Text"))
		{
			textMaterial->setShader(textShader);
			textMaterial->createTextureArrayParameter("textTextureArray", nullptr);
			textMaterial->createVec4Parameter("spriteColor", glm::vec4(0.0f));

			materialsMap.emplace(typeid(TextBlock), textMaterial);
		}
	}
}

void UIContext::pushContextToRender(SpriteRenderer* renderer)
{
	for (auto iter = spawnedPanels.rbegin(); iter != spawnedPanels.rend(); ++iter)
	{
		if (Panel* panel = iter->get())
		{
			// panel->refresh();
			// panel->pushToRenderer(renderer);
		}
	}
}

void UIContext::onWindowResize(std::shared_ptr<WindowResizeEvent> event)
{
	for (auto& panel : spawnedPanels)
	{
		if (panel)
		{
			panel->refresh();
		}
	}
}
