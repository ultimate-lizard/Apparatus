#include "Widget.h"

#include "../../Apparatus.h"
#include "../../Core/Logger.h"

Widget::Widget() :
    parent(nullptr),
    position(0),
    size(0),
    horizontalAlignment(Alignment::Left),
    verticalAlignment(Alignment::Left),
    margins({ 0 }),
    sizeToContent(false),
    contentSize(0),
    invalidated(true),
    mouseCaptureEnabled(false),
    visible(true),
    containsCursor(false)
{
}

bool Widget::refresh()
{
    for (Widget* child : children)
    {
        child->refresh();
    }

    const bool wasInvalidated = invalidated;

    if (sizeToContent && wasInvalidated)
    {
        glm::ivec2 maxSize(0);

        for (Widget* child : children)
        {
            const glm::ivec2 childSize = child->getGlobalSize();
            if (glm::all(glm::greaterThan(childSize, maxSize)))
            {
                maxSize = childSize;
            }
        }

        contentSize = maxSize;

        invalidated = false;
    }

    return wasInvalidated;
}

glm::ivec2 Widget::getGlobalPosition()
{
    glm::ivec2 globalPosition = getPosition();

    glm::ivec2 parentSize = Apparatus::getWindow().getWindowSize();

    if (parent)
    {
        globalPosition = globalPosition + parent->getGlobalPosition();
        parentSize = parent->getGlobalSize();
    }

    // Apply alignment
    if (horizontalAlignment == Alignment::Center)
    {
        globalPosition.x += parentSize.x / 2 - getGlobalSize().x / 2;
    }
    else if (horizontalAlignment == Alignment::Right)
    {
        globalPosition.x += parentSize.x - getGlobalSize().x;
    }

    if (verticalAlignment == Alignment::Center)
    {
        globalPosition.y += parentSize.y / 2 - getGlobalSize().y / 2;
    }
    else if (verticalAlignment == Alignment::Right)
    {
        globalPosition.y += parentSize.y - getGlobalSize().y;
    }

    // Apply margin
    if (horizontalAlignment == Alignment::Left)
    {
        globalPosition.x += getMargin(Widget::Side::Left);
    }
    else if (horizontalAlignment == Alignment::Right)
    {
        globalPosition.x -= getMargin(Widget::Side::Right);
    }
    else if (horizontalAlignment == Alignment::Fill)
    {
        globalPosition.x += getMargin(Widget::Side::Left);
    }

    if (verticalAlignment == Alignment::Left)
    {
        globalPosition.y += getMargin(Widget::Side::Top);
    }
    else if (verticalAlignment == Alignment::Right)
    {
        globalPosition.y -= getMargin(Widget::Side::Bottom);
    }
    else if (verticalAlignment == Alignment::Fill)
    {
        globalPosition.y += getMargin(Widget::Side::Top);
    }

    return globalPosition;
}

glm::ivec2 Widget::getGlobalSize()
{
    glm::ivec2 size = getSize();

    if (sizeToContent)
    {
        size = contentSize;
    }

    glm::ivec2 parentSize = Apparatus::getWindow().getWindowSize();

    if (parent)
    {
        parentSize = parent->getSize();
    }

    if (horizontalAlignment == Alignment::Fill)
    {
        size.x = parentSize.x - getMargin(Widget::Side::Left) - getMargin(Widget::Side::Right);
    }

    if (verticalAlignment == Alignment::Fill)
    {
        size.y = parentSize.y - getMargin(Widget::Side::Top) - getMargin(Widget::Side::Bottom);
    }

    if (horizontalAlignment != Alignment::Fill && verticalAlignment != Alignment::Fill)
    {
        size.x += getMargin(Widget::Side::Left);
        size.x += getMargin(Widget::Side::Right);
        size.y += getMargin(Widget::Side::Top);
        size.y += getMargin(Widget::Side::Bottom);
    }

    return size;
}

void Widget::setSize(const glm::ivec2& size)
{
    this->size = size;
    invalidate();
}

const glm::ivec2& Widget::getSize() const
{
    return size;
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
        child->invalidate();
        invalidate();

        child->setVisibility(isVisible());
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
    invalidate();
}

const glm::ivec2& Widget::getPosition() const
{
    return position;
}

void Widget::setHorizontalAlignment(Alignment alignment)
{
    horizontalAlignment = alignment;
    invalidate();
}

void Widget::setVerticalAlignment(Alignment alignment)
{
    verticalAlignment = alignment;
    invalidate();
}

void Widget::setMargin(Side side, int margin)
{
    margins[static_cast<size_t>(side)] = margin;
    invalidate();
}

void Widget::setMargin(int margin)
{
    for (size_t i = 0; i < 4; ++i)
    {
        margins[i] = margin;
    }

    invalidate();
}

int Widget::getMargin(Side side) const
{
    return margins[static_cast<size_t>(side)];
}

void Widget::setSizeToContentEnabled(bool enabled)
{
    this->sizeToContent = enabled;
    invalidate();
}

bool Widget::isSizeToContentEnabled() const
{
    return sizeToContent == true;
}

void Widget::invalidate()
{
    invalidated = true;

    for (Widget* child : children)
    {
        child->invalidate();
    }
}

void Widget::setMouseCaptureEnabled(bool enabled)
{
    this->mouseCaptureEnabled = enabled;
}

bool Widget::isMouseCaptureEnabled() const
{
    return mouseCaptureEnabled;
}

void Widget::setVisibility(bool visible)
{
    this->visible = visible;

    for (Widget* child : children)
    {
        child->setVisibility(visible);
    }

    invalidate();
}

bool Widget::isVisible() const
{
    return visible;
}

void Widget::onMouseMove(const glm::ivec2& cursorPosition)
{
    if (isContaining(cursorPosition))
    {
        if (!containsCursor)
        {
            containsCursor = true;
            onMouseEnter();
            // LOG("On mouse enter " + getName(), LogLevel::Info);
        }
    }
    else
    {
        if (containsCursor)
        {
            containsCursor = false;
            onMouseLeave();
            // LOG("On mouse leave " + getName(), LogLevel::Info);
        }
    }
}

bool Widget::onKeyInput(InputKey key, KeyEventType type)
{
    const glm::ivec2 cursorPosition = Apparatus::getWindow().getMouseCursorPosition();
    return isContaining(cursorPosition) && mouseCaptureEnabled && isVisible() && key == InputKey::MouseLeftButton;
}

bool Widget::isContaining(const glm::ivec2& coordiante)
{
    return getGlobalPosition().x < coordiante.x && getGlobalPosition().x + getGlobalSize().x > coordiante.x &&
        getGlobalPosition().y < coordiante.y && getGlobalPosition().y + getGlobalSize().y > coordiante.y;
}
