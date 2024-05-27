#include "NinePatchPanel.h"

#include "../../Rendering/Material.h"
#include "../../Apparatus.h"
#include "../../Core/AssetManager/AssetManager.h"

NinePatchPanel::NinePatchPanel() :
	borders({})
{

}

void NinePatchPanel::init()
{
	setMouseCaptureEnabled(true);

	if (sprite = std::make_unique<Sprite>())
	{
		if (AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>())
		{
			sprite->setMaterial(assetManager->findAsset<Material>("Material_NinePatchPanel"));
			invalidate();
		}
	}
}

void NinePatchPanel::refresh()
{
	Widget::refresh();

	if (invalidated && sprite)
	{
		sprite->setPosition(getGlobalPosition());
		sprite->setSize(getGlobalSize());
		sprite->rebuildMesh();

		if (Material* spriteMaterial = sprite->getMaterial())
		{
			MaterialParameters& params = spriteMaterial->getParameters();

			glm::ivec4 borders;
			borders.x = getBorder(ImagePanel::Side::Left);
			borders.y = getBorder(ImagePanel::Side::Right);
			borders.z = getBorder(ImagePanel::Side::Top);
			borders.w = getBorder(ImagePanel::Side::Bottom);

			params.setVec4("borders", borders);

			params.setVec2("textureBorderSize", sprite->getTextureSize());
			params.setVec2("spriteSize", getGlobalSize());
		}

		invalidated = false;
	}
}

void NinePatchPanel::setBorder(Side side, int border)
{
	borders[static_cast<int>(side)] = border;
	invalidate();
}

void NinePatchPanel::setBorder(int border)
{
	for (size_t i = 0; i < 4; ++i)
	{
		borders[i] = border;
	}
}

int NinePatchPanel::getBorder(Side side) const
{
	return borders[static_cast<int>(side)];
}
