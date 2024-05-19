#include "Panel.h"

#include "../UIContext.h"
#include "../Font.h"
#include "../../Rendering/Texture.h"
#include "../../Rendering/Material.h"
#include "../../Apparatus.h"
#include "../../Rendering/Sprite/TextBlock.h"

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

void Widget::addChild(Widget* child)
{
    if (child)
    {
        if (Widget* parent = child->getParent())
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

Widget* Widget::getChild(size_t index) const
{
    if (index < children.size())
    {
        return children[index];
    }

    return nullptr;
}

size_t Widget::getChildrenCount() const
{
    return children.size();
}

void Widget::setPosition(const glm::ivec2& position)
{
    this->position = position;
}

const glm::ivec2& Widget::getPosition() const
{
    return position;
}

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

void Widget::setName(const std::string& name)
{
    this->name = name;
}

const std::string& Widget::getName() const
{
    return name;
}

Widget* Widget::getParent()
{
    return parent;
}

void Widget::setHorizontalAlignment(Alignment alignment)
{
    horizontalAlignment = alignment;
}

void Widget::setVerticalAlignment(Alignment alignment)
{
    verticalAlignment = alignment;
}

void BoxModelPanel::setMargin(Side side, int margin)
{
    margins[static_cast<size_t>(side)] = margin;
}

int BoxModelPanel::getMargin(Side side) const
{
    return margins[static_cast<size_t>(side)];
}

void Widget::refresh()
{
    for (Widget* child : children)
    {
        child->refresh();
    }
}

glm::ivec2 Widget::getGlobalPosition() const
{
    glm::ivec2 position = getPosition();

    glm::ivec2 parentSize = Apparatus::getWindow().getWindowSize();

    if (parent)
    {
        position = position + parent->getGlobalPosition();
        parentSize = parent->getGlobalSize();
    }

    // Apply alignment
    if (horizontalAlignment == Alignment::Center)
    {
        position.x += parentSize.x / 2 - getGlobalSize().x / 2;
    }
    else if (horizontalAlignment == Alignment::Right)
    {
        position.x += parentSize.x - getGlobalSize().x;
    }

    if (verticalAlignment == Alignment::Center)
    {
        position.y += parentSize.y / 2 - getGlobalSize().y / 2;
    }
    else if (verticalAlignment == Alignment::Right)
    {
        position.y += parentSize.y - getGlobalSize().y;
    }

    return position;
}

BoxModelPanel::BoxModelPanel() :
    size(0),
    margins({0})
{
}

glm::ivec2 BoxModelPanel::getGlobalPosition() const
{
    glm::ivec2 globalPosition = Widget::getGlobalPosition();

    // Apply margin
    if (horizontalAlignment == Alignment::Left)
    {
        globalPosition.x += getMargin(ImagePanel::Side::Left);
    }
    else if (horizontalAlignment == Alignment::Right)
    {
        globalPosition.x -= getMargin(ImagePanel::Side::Right);
    }
    else if (horizontalAlignment == Alignment::Fill)
    {
        globalPosition.x += getMargin(ImagePanel::Side::Left);
    }

    if (verticalAlignment == Alignment::Left)
    {
        globalPosition.y += getMargin(ImagePanel::Side::Top);
    }
    else if (verticalAlignment == Alignment::Right)
    {
        globalPosition.y -= getMargin(ImagePanel::Side::Bottom);
    }
    else if (verticalAlignment == Alignment::Fill)
    {
        globalPosition.y += getMargin(ImagePanel::Side::Top);
    }

    return globalPosition;
}

glm::ivec2 BoxModelPanel::getGlobalSize() const
{
    glm::ivec2 size = getSize();

    glm::ivec2 parentSize = Apparatus::getWindow().getWindowSize();

    if (parent)
    {
        parentSize = parent->getGlobalSize();
    }

    if (horizontalAlignment == Alignment::Fill)
    {
        size.x = parentSize.x - getMargin(ImagePanel::Side::Left) - getMargin(ImagePanel::Side::Right);
    }

    if (verticalAlignment == Alignment::Fill)
    {
        size.y = parentSize.y - getMargin(ImagePanel::Side::Top) - getMargin(ImagePanel::Side::Bottom);
    }

    return size;
}

Widget::Widget() :
    parent(nullptr),
    position(0),
    horizontalAlignment(Alignment::Left),
    verticalAlignment(Alignment::Left)
{
}

TextBlock* TextPanel::getTextBlock()
{
    return textBlock.get();
}

void TextPanel::setText(const std::string& text)
{
    if (textBlock)
    {
        textBlock->setText(text);
    }
}

void TextPanel::setColor(const glm::vec4& color)
{
    if (textBlock)
    {
        textBlock->setColor(color);
    }
}

void TextPanel::setFontSize(float fontSize)
{
    if (textBlock)
    {
        textBlock->setFontSize(fontSize);
    }
}

void TextPanel::setDepth(float depth)
{
    textBlock->setDepth(depth);
}

void TextPanel::init()
{
    if (textBlock = std::make_unique<TextBlock>())
    {
        Font* defaultFont = Apparatus::getAssetManager().findAsset<Font>("Font_Arial");
        textBlock->setFont(defaultFont);
        textBlock->setMaterial(Apparatus::getAssetManager().findAsset<Material>("Material_TextPanel"));
    }
}

void TextPanel::refresh()
{
    Widget::refresh();

    if (textBlock)
    {
        textBlock->setPosition(getGlobalPosition());
        textBlock->setSize(getGlobalSize());
        textBlock->rebuildMesh();
    }
}

void TextPanel::render(SpriteRenderer* renderer)
{
    if (renderer)
    {
        renderer->push(textBlock.get());
    }
}

void HorizontalPanel::addChild(Widget* child)
{
    std::unique_ptr<BoxModelPanel> childContainer = std::make_unique<BoxModelPanel>();
    childContainer->addChild(child);
    childContainers.push_back(std::move(childContainer));
}

glm::ivec2 HorizontalPanel::getGlobalSize() const
{
    glm::ivec2 size(0);
    int maxHeight = 0;

    for (const std::unique_ptr<BoxModelPanel>& childContainer : childContainers)
    {
        if (Widget* child = childContainer->getChild(0))
        {
            // Accumulate the overall X size
            const glm::ivec2 childSize = child->getGlobalSize();
            size.x += childSize.x;

            // Find the largest in Y widget
            if (childSize.y > maxHeight)
            {
                maxHeight = childSize.y;
            }
        }
    }

    size.y = maxHeight;

    return size;
}

void HorizontalPanel::refresh()
{
    Widget::refresh();

    float offset = 0.0f;

    for (const std::unique_ptr<BoxModelPanel>& childContainer : childContainers)
    {
        if (Widget* child = childContainer->getChild(0))
        {
            childContainer->setSize(child->getGlobalSize());
            childContainer->setPosition(getGlobalPosition() + glm::ivec2(offset, 0));
            childContainer->refresh();

            offset += childContainer->getGlobalSize().x;
        }
    }
}

void BoxModelPanel::setSize(const glm::ivec2& size)
{
    this->size = size;
}

const glm::ivec2& BoxModelPanel::getSize() const
{
    return size;
}
