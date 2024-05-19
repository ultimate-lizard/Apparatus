#include "NinePatchPanel.h"

#include "../../Rendering/Material.h"
#include "../../Apparatus.h"

NinePatchPanel::NinePatchPanel() :
	borders({})
{

}

void NinePatchPanel::setBorder(Side side, int border)
{
	borders[static_cast<int>(side)] = border;
}

int NinePatchPanel::getBorder(Side side) const
{
	return borders[static_cast<int>(side)];
}

void NinePatchPanel::init()
{
	if (sprite = std::make_unique<Sprite>())
	{
		sprite->setMaterial(Apparatus::getAssetManager().findAsset<Material>("Material_NinePatchPanel"));
	}
}

void NinePatchPanel::refresh()
{
	ImagePanel::refresh();

	if (sprite)
	{
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
	}
}
