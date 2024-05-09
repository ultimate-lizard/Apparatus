#include "Sprite.h"

#include "../Texture.h"
#include "../Material.h"
#include "../../UI/UIContext.h"

Sprite::Sprite(int bufferSize) :
    Drawable(bufferSize),
    texture(nullptr),
    texturePosition(0),
    textureSize(0)
{

}

void Sprite::setTexture(Texture* texture)
{
    this->texture = texture;

    if (texture)
    {
        setSize(texture->getSize());
        setTextureSize(texture->getSize());
    }

    if (material)
    {
        MaterialParameters& params = material->getParameters();
        params.setTexture("spriteTexture", texture);
    }
}

Texture* Sprite::getTexture()
{
    return texture;
}

void Sprite::setTexturePosition(const glm::ivec2& coordinates)
{
    this->texturePosition = coordinates;
}

const glm::ivec2& Sprite::getTexturePosition() const
{
    return texturePosition;
}

void Sprite::setTextureSize(const glm::ivec2& size)
{
    this->textureSize = size;
}

const glm::ivec2& Sprite::getTextureSize() const
{
    return textureSize;
}

void Sprite::rebuildMesh()
{
    if (!spriteMesh)
    {
        return;
    }

    const glm::vec2 spriteSize = getSize();
    const glm::vec2 spritePosition = getPosition();
    const glm::vec2 texturePosition = getTexturePosition();
    const glm::vec2 textureBorderSize = getTextureSize();
    const float spriteDepth = getDepth();

    const float textureMinX = texturePosition.x / textureBorderSize.x;
    const float textureMinY = texturePosition.y / textureBorderSize.y;
    const float textureMaxX = (texturePosition.x + spriteSize.x) / textureBorderSize.x;
    const float textureMaxY = (texturePosition.y + spriteSize.y) / textureBorderSize.y;

    const std::vector<float> spriteVertices = {
        1.0f * spriteSize.x + spritePosition.x, 0.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMaxX, textureMinY,
        0.0f * spriteSize.x + spritePosition.x, 0.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMinX, textureMinY,
        0.0f * spriteSize.x + spritePosition.x, 1.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMinX, textureMaxY,

        1.0f * spriteSize.x + spritePosition.x, 0.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMaxX, textureMinY,
        0.0f * spriteSize.x + spritePosition.x, 1.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMinX, textureMaxY,
        1.0f * spriteSize.x + spritePosition.x, 1.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMaxX, textureMaxY,
    };

    spriteMesh->bind();
    spriteMesh->setSubData(spriteVertices);
}
