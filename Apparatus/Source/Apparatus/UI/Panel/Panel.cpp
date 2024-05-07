#include "Panel.h"

#include "../../Rendering/Texture.h"
#include "../../Apparatus.h"

Panel::Panel(Material* material, Texture* texture) :
    sprite(material, texture),
    parent(nullptr),
    position(sprite.getPosition()),
    size(sprite.getSize()),
    horizontalAlignment(Alignment::Left),
    verticalAlignment(Alignment::Left),
    margins({0}),
    paddings({0})
{
}

void Panel::addChild(Panel* child)
{
    if (child)
    {
        if (Panel* parent = child->getParent())
        {
            auto childToDeleteIter = std::find(parent->children.begin(), parent->children.end(), child);
            if (childToDeleteIter != parent->children.end())
            {
                parent->children.erase(childToDeleteIter);
            }
        }

        children.push_back(child);
        child->parent = this;
    }
}

void Panel::setTexture(Texture* texture)
{
    sprite.setTexture(texture);
}

Texture* Panel::getTexture()
{
    return sprite.getTexture();
}

void Panel::setMaterial(Material* material)
{
    sprite.setMaterial(material);
}

Material* Panel::getMaterial()
{
    return sprite.getMaterial();
}

void Panel::setPosition(const glm::vec2& position)
{
    this->position = position;
}

const glm::vec2& Panel::getPosition() const
{
    return position;
}

void Panel::setSize(const glm::vec2& size)
{
    this->size = size;
}

const glm::vec2& Panel::getSize() const
{
    return size;
}

void Panel::setTexturePosition(const glm::vec2& texturePosition)
{
    sprite.setTexturePosition(texturePosition);
}

glm::vec2 Panel::getTexturePosition() const
{
    return sprite.getTexturePosition();
}

void Panel::setTextureSize(const glm::vec2& textureSize)
{
    sprite.setTextureSize(textureSize);
}

glm::vec2 Panel::getTextureSize() const
{
    return sprite.getTextureSize();
}

void Panel::setDepth(float depth)
{
    sprite.setDepth(depth);
}

float Panel::getDepth() const
{
    return sprite.getDepth();
}

void Panel::setColor(const glm::vec4& color)
{
    sprite.setColor(color);
}

glm::vec4 Panel::getColor() const
{
    return sprite.getColor();
}

void Panel::pushToRenderer(SpriteRenderer* renderer)
{
    if (renderer)
    {
        renderer->push(&sprite);
    }
}

void Panel::refresh()
{
    sprite.setPosition(calculateSpritePosition());
    sprite.setSize(calculateSpriteSize());
    sprite.updateMesh();
}

Panel* Panel::getParent()
{
    return parent;
}

void Panel::setHorizontalAlignment(Alignment alignment)
{
    horizontalAlignment = alignment;
}

void Panel::setVerticalAlignment(Alignment alignment)
{
    verticalAlignment = alignment;
}

void Panel::setMargin(Side side, int margin)
{
    margins[static_cast<size_t>(side)] = margin;
}

int Panel::getMargin(Side side) const
{
    return margins[static_cast<size_t>(side)];
}

void Panel::setPadding(Side side, int padding)
{
    paddings[static_cast<size_t>(side)] = padding;
}

int Panel::getPadding(Side side) const
{
    return paddings[static_cast<size_t>(side)];
}

glm::vec2 Panel::calculateSpritePosition() const
{
    glm::vec2 position = getPosition();

    glm::vec2 parentSize = Apparatus::getWindow().getWindowSize();

    if (parent)
    {
        position = parent->calculateSpritePosition();
        parentSize = parent->calculateSpriteSize();

        // Apply padding
        if (horizontalAlignment == Alignment::Left || horizontalAlignment == Alignment::Fill)
        {
            position.x += parent->getPadding(Panel::Side::Left);
        }

        if (verticalAlignment == Alignment::Left || verticalAlignment == Alignment::Fill)
        {
            position.y += parent->getPadding(Panel::Side::Top);
        }

        if (horizontalAlignment == Alignment::Right)
        {
            position.x -= parent->getPadding(Panel::Side::Right);
        }

        if (verticalAlignment == Alignment::Right)
        {
            position.y -= parent->getPadding(Panel::Side::Bottom);
        }
    }

    // Apply alignment
    if (horizontalAlignment == Alignment::Center)
    {
        position.x += parentSize.x / 2 - calculateSpriteSize().x / 2;
    }
    else if (horizontalAlignment == Alignment::Right)
    {
        position.x += parentSize.x - calculateSpriteSize().x;
    }

    if (verticalAlignment == Alignment::Center)
    {
        position.y += parentSize.y / 2 - calculateSpriteSize().y / 2;
    }
    else if (verticalAlignment == Alignment::Right)
    {
        position.y += parentSize.y - calculateSpriteSize().y;
    }

    // Apply margin
    if (horizontalAlignment == Alignment::Left)
    {
        position.x += getMargin(Panel::Side::Left);
    }
    else if (horizontalAlignment == Alignment::Right)
    {
        position.x -= getMargin(Panel::Side::Right);
    }
    else if (horizontalAlignment == Alignment::Fill)
    {
        position.x += getMargin(Panel::Side::Left);
    }
    
    if (verticalAlignment == Alignment::Left)
    {
        position.y += getMargin(Panel::Side::Top);
    }
    else if (verticalAlignment == Alignment::Right)
    {
        position.y -= getMargin(Panel::Side::Bottom);
    }
    else if (verticalAlignment == Alignment::Fill)
    {
        position.y += getMargin(Panel::Side::Top);
    }

    return position;
}

glm::vec2 Panel::calculateSpriteSize() const
{
    glm::vec2 size = getSize();

    glm::vec2 parentSize = Apparatus::getWindow().getWindowSize();

    if (parent)
    {
        parentSize = parent->calculateSpriteSize();
    }

    if (horizontalAlignment == Alignment::Fill)
    {
        size.x = parentSize.x - getMargin(Panel::Side::Left) - getMargin(Panel::Side::Right);
    }

    if (verticalAlignment == Alignment::Fill)
    {
        size.y = parentSize.y - getMargin(Panel::Side::Top) - getMargin(Panel::Side::Bottom);
    }

    if (parent)
    {
        if (horizontalAlignment == Alignment::Fill)
        {
            size.x -= parent->getPadding(Panel::Side::Left) + parent->getPadding(Panel::Side::Right);
        }

        if (verticalAlignment == Alignment::Fill)
        {
            size.y -= parent->getPadding(Panel::Side::Top) + parent->getPadding(Panel::Side::Bottom);
        }
    }

    return size;
}
