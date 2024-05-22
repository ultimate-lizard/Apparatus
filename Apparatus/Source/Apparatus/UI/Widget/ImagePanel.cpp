#include "ImagePanel.h"

#include "../../Apparatus.h"
#include "../../Rendering/Material.h"

void ImagePanel::init()
{
    if (sprite = std::make_unique<Sprite>())
    {
        sprite->setMaterial(Apparatus::getAssetManager().findAsset<Material>("Material_Panel"));
    }
}

void ImagePanel::refresh()
{
    Widget::refresh();

    if (sprite)
    {
        sprite->setPosition(getGlobalPosition());
        sprite->setSize(getGlobalSize());
        sprite->rebuildMesh();
    }
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
    }
}

void ImagePanel::setDepth(float depth)
{
    if (sprite)
    {
        sprite->setDepth(depth);
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
