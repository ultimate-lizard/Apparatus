#include "ImagePanel.h"

#include "../../Apparatus.h"
#include "../../Rendering/Material.h"
#include "../../Rendering/Texture.h"
#include "../../Core/AssetManager/AssetManager.h"

void ImagePanel::init()
{
    setMouseCaptureEnabled(true);

    if (sprite = std::make_unique<Sprite>())
    {
        if (AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>())
        {
            sprite->setMaterial(assetManager->findAsset<Material>("Material_Panel"));
            invalidate();
        }
    }
}

bool ImagePanel::refresh()
{
    bool wasInvalidated = BoxModelPanel::refresh();

    if (wasInvalidated)
    {
        if (sprite)
        {
            sprite->setPosition(getGlobalPosition());
            sprite->setSize(getGlobalSize());
            sprite->rebuildMesh();
        }

        invalidated = false;
    }

    return wasInvalidated;
}

void ImagePanel::render(SpriteRenderer* renderer)
{
    if (renderer)
    {
        renderer->push(sprite.get());
    }
}

Sprite* ImagePanel::getSprite()
{
    return sprite.get();
}

void ImagePanel::setTexture(Texture* texture)
{
    if (sprite)
    {
        sprite->setTexture(texture);

        if (size == glm::ivec2(0))
        {
            size = texture->getSize();
        }

        invalidate();
    }
}

void ImagePanel::setDepth(float depth)
{
    if (sprite)
    {
        sprite->setDepth(depth);
        invalidate();
    }
}

float ImagePanel::getDepth() const
{
    if (sprite)
    {
        return sprite->getDepth();
    }

    return 0.0f;
}
