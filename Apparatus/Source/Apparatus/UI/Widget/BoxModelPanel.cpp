#include "BoxModelPanel.h"

#include "ImagePanel.h"
#include "../../Apparatus.h"

BoxModelPanel::BoxModelPanel() :
    size(0),
    margins({ 0 })
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

void BoxModelPanel::setSize(const glm::ivec2& size)
{
    this->size = size;
}

const glm::ivec2& BoxModelPanel::getSize() const
{
    return size;
}

void BoxModelPanel::setMargin(Side side, int margin)
{
    margins[static_cast<size_t>(side)] = margin;
}

int BoxModelPanel::getMargin(Side side) const
{
    return margins[static_cast<size_t>(side)];
}
