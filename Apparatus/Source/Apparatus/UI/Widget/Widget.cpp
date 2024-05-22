#include "Widget.h"

#include "../../Apparatus.h"

Widget::Widget() :
    parent(nullptr),
    position(0),
    horizontalAlignment(Alignment::Left),
    verticalAlignment(Alignment::Left)
{
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

void Widget::setHorizontalAlignment(Alignment alignment)
{
    horizontalAlignment = alignment;
}

void Widget::setVerticalAlignment(Alignment alignment)
{
    verticalAlignment = alignment;
}