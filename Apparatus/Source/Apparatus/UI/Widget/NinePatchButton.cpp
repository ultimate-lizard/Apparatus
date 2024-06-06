#include "NinePatchButton.h"

#include "../../Apparatus.h"
#include "../../Rendering/Material.h"
#include "../../Rendering/Sprite/Sprite.h"
#include "../../Core/AssetManager/AssetManager.h"
#include "ImagePanel.h"

class Material;

bool NinePatchButton::refresh()
{
    bool wasInvalidated = Button::refresh();

    std::vector<Sprite*> sprites;
    sprites.push_back(idleSprite.get());
    sprites.push_back(hoverSprite.get());
    sprites.push_back(pressSprite.get());

    for (Sprite* sprite : sprites)
    {
        if (!sprite)
        {
            continue;
        }

        Material* spriteMaterial = sprite->getMaterial();
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

    return wasInvalidated;
}

void NinePatchButton::setBorder(Side side, int border)
{
    borders[static_cast<int>(side)] = border;
    invalidate();
}

void NinePatchButton::setBorder(int border)
{
    for (size_t i = 0; i < 4; ++i)
    {
        borders[i] = border;
    }
}

int NinePatchButton::getBorder(Side side) const
{
    return borders[static_cast<int>(side)];
}

void NinePatchButton::initMaterial()
{
    AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
    Material* buttonMaterial = assetManager->findAsset<Material>("Material_NinePatchPanel");
    idleSprite->setMaterial(buttonMaterial);
    hoverSprite->setMaterial(buttonMaterial);
    pressSprite->setMaterial(buttonMaterial);
}

void NinePatchButton::refreshSprite()
{
    if (!currentSprite)
    {
        return;
    }

    currentSprite->setPosition(getGlobalPosition());
    currentSprite->setSize(getGlobalSize());
    currentSprite->rebuildMesh();
}
