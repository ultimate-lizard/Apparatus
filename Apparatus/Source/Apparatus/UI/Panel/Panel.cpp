#include "Panel.h"

#include "../../Rendering/Texture.h"
#include "../../Apparatus.h"
#include "../../Rendering/Sprite/TextBlock.h"

Panel::Panel() :
    sprite(nullptr)
{
}

void Panel::setSprite(Sprite* sprite)
{
    this->sprite = sprite;
}

Sprite* Panel::getSprite()
{
    return sprite;
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

void Panel::refresh()
{
    Widget::refresh();

    if (sprite)
    {
        sprite->setPosition(getGlobalPosition());
        sprite->setSize(getGlobalSize());
        sprite->rebuildMesh();
    }
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

void BoxModelWidget::setMargin(Side side, int margin)
{
    margins[static_cast<size_t>(side)] = margin;
}

int BoxModelWidget::getMargin(Side side) const
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

glm::ivec2 BoxModelWidget::getGlobalPosition() const
{
    glm::ivec2 globalPosition = Widget::getGlobalPosition();

    // Apply margin
    if (horizontalAlignment == Alignment::Left)
    {
        globalPosition.x += getMargin(Panel::Side::Left);
    }
    else if (horizontalAlignment == Alignment::Right)
    {
        globalPosition.x -= getMargin(Panel::Side::Right);
    }
    else if (horizontalAlignment == Alignment::Fill)
    {
        globalPosition.x += getMargin(Panel::Side::Left);
    }

    if (verticalAlignment == Alignment::Left)
    {
        globalPosition.y += getMargin(Panel::Side::Top);
    }
    else if (verticalAlignment == Alignment::Right)
    {
        globalPosition.y -= getMargin(Panel::Side::Bottom);
    }
    else if (verticalAlignment == Alignment::Fill)
    {
        globalPosition.y += getMargin(Panel::Side::Top);
    }

    return globalPosition;
}

glm::ivec2 BoxModelWidget::getGlobalSize() const
{
    glm::ivec2 size = getSize();

    glm::ivec2 parentSize = Apparatus::getWindow().getWindowSize();

    if (parent)
    {
        parentSize = parent->getGlobalSize();
    }

    if (horizontalAlignment == Alignment::Fill)
    {
        size.x = parentSize.x - getMargin(Panel::Side::Left) - getMargin(Panel::Side::Right);
    }

    if (verticalAlignment == Alignment::Fill)
    {
        size.y = parentSize.y - getMargin(Panel::Side::Top) - getMargin(Panel::Side::Bottom);
    }

    return size;
}

Widget::Widget() :
    parent(nullptr),
    position(0),
    horizontalAlignment(Alignment::Left),
    verticalAlignment(Alignment::Left),
    margins({ 0 })
{
}

void TextPanel::setTextBlock(TextBlock* textBlock)
{
    this->textBlock = textBlock;

    if (textBlock)
    {
        setSize(textBlock->getSize());
    }
}

TextBlock* TextPanel::getTextBlock()
{
    return textBlock;
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

void HorizontalPanel::addChild(Widget* child)
{
    std::unique_ptr<BoxModelWidget> childContainer = std::make_unique<BoxModelWidget>();
    childContainer->addChild(child);
    childContainers.push_back(std::move(childContainer));
}

glm::ivec2 HorizontalPanel::getGlobalSize() const
{
    glm::ivec2 size(0);
    int maxHeight = 0;

    for (const std::unique_ptr<BoxModelWidget>& childContainer : childContainers)
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

    for (const std::unique_ptr<BoxModelWidget>& childContainer : childContainers)
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

void BoxModelWidget::setSize(const glm::ivec2& size)
{
    this->size = size;
}

const glm::ivec2& BoxModelWidget::getSize() const
{
    return size;
}
