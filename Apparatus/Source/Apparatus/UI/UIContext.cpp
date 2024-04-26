#include "UIContext.h"

#include "../Apparatus.h"
#include "../Rendering/Material.h"
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

			materialsMap.emplace(typeid(NinePatchPanel), ninePatchMaterial);
		}
	}
}

void UIContext::pushContextToRender(SpriteRenderer* renderer)
{
	for (auto iter = spawnedPanels.rbegin(); iter != spawnedPanels.rend(); ++iter)
	{
		if (Panel* panel = iter->get())
		{
			panel->refresh();
			panel->pushToRenderer(renderer);
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
