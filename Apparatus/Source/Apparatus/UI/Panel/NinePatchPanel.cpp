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

void NinePatchPanel::refresh()
{
	Panel::refresh();

	if (sprite)
	{
		if (Material* spriteMaterial = sprite->getMaterial())
		{
			MaterialParameters& params = spriteMaterial->getParameters();

			glm::ivec4 borders;
			borders.x = getBorder(Panel::Side::Left);
			borders.y = getBorder(Panel::Side::Right);
			borders.z = getBorder(Panel::Side::Top);
			borders.w = getBorder(Panel::Side::Bottom);

			params.setVec4("borders", borders);

			params.setVec2("textureBorderSize", sprite->getTextureSize());
			params.setVec2("spriteSize", getGlobalSize());
		}
	}
}
