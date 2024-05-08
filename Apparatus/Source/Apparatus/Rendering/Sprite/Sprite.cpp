#include "Sprite.h"

#include "../Material.h"
#include "../Texture.h"

Sprite::Sprite(Material* material, Texture* texture) :
    texture(texture),
    material(material),
    position(0),
    size(0),
    depth(0.0f),
    texturePosition(0),
    textureBorderSize(0),
    color(0.0f)
{
    if (texture)
    {
        size = texture->getSize();
        textureBorderSize = texture->getSize();
    }

    if (material)
    {
        setMaterial(material->createInstance());
    }

    spriteMesh = std::make_unique<SpriteMesh>(256);
}

void Sprite::setMaterial(Material* material)
{
    this->material = material;
}

Material* Sprite::getMaterial()
{
    return material;
}

void Sprite::setTexture(Texture* texture)
{
    this->texture = texture;
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
    this->textureBorderSize = size;
}

const glm::ivec2& Sprite::getTextureSize() const
{
    return textureBorderSize;
}

void Sprite::setColor(const glm::vec4& color)
{
    this->color = color;
}

const glm::vec4& Sprite::getColor() const
{
    return color;
}

void Sprite::setPosition(const glm::ivec2& position)
{
    this->position = position;
}

const glm::ivec2& Sprite::getPosition() const
{
    return position;
}

void Sprite::setSize(const glm::ivec2& scale)
{
    this->size = scale;
}

const glm::ivec2& Sprite::getSize() const
{
    return size;
}

void Sprite::setDepth(float depth)
{
    this->depth = depth;
}

float Sprite::getDepth() const
{
    return depth;
}

SpriteMesh* Sprite::getSpriteMesh()
{
    return spriteMesh.get();
}

void Sprite::updateMesh()
{
    if (spriteMesh)
    {
        const glm::vec2 spriteSize = getSize();
        const glm::vec2 spritePosition = getPosition();
        const glm::vec2 texturePosition = getTexturePosition();
        const glm::vec2 textureBorderSize = getTextureSize();
        const glm::vec4 spriteColor = getColor();
        const float spriteDepth = getDepth();

        const float textureMinX = texturePosition.x / textureBorderSize.x;
        const float textureMinY = texturePosition.y / textureBorderSize.y;
        const float textureMaxX = (texturePosition.x + spriteSize.x) / textureBorderSize.x;
        const float textureMaxY = (texturePosition.y + spriteSize.y) / textureBorderSize.y;

        const std::vector<float> spriteVertices = {
            1.0f * spriteSize.x + spritePosition.x, 0.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMaxX, textureMinY, spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w,
            0.0f * spriteSize.x + spritePosition.x, 0.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMinX, textureMinY, spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w,
            0.0f * spriteSize.x + spritePosition.x, 1.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMinX, textureMaxY, spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w,
            1.0f * spriteSize.x + spritePosition.x, 0.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMaxX, textureMinY, spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w,
            0.0f * spriteSize.x + spritePosition.x, 1.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMinX, textureMaxY, spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w,
            1.0f * spriteSize.x + spritePosition.x, 1.0f * spriteSize.y + spritePosition.y, spriteDepth, textureMaxX, textureMaxY, spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w,
        };

        spriteMesh->bind();
        spriteMesh->setSubData(spriteVertices);
    }

    if (material)
    {
        MaterialParameters& params = material->getParameters();
        params.setTexture("spriteTexture", texture);
    }
}
