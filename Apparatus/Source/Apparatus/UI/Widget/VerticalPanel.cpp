#include "VerticalPanel.h"

VerticalPanel::VerticalPanel() :
    offset(0)
{
}

void VerticalPanel::addChild(Widget* child)
{
    std::unique_ptr<Widget> childContainer = std::make_unique<Widget>();

    childContainer->setVisibility(isVisible());

    childContainer->addChild(child);
    childContainers.push_back(std::move(childContainer));
    child->invalidate();
    invalidate();
}

glm::ivec2 VerticalPanel::getGlobalSize()
{
    glm::ivec2 size(0);
    int maxWidth = 0;

    for (const std::unique_ptr<Widget>& childContainer : childContainers)
    {
        if (Widget* child = childContainer->getChild(0))
        {
            // Accumulate the overall X size
            const glm::ivec2 childSize = child->getGlobalSize();
            size.y += childSize.y;

            // Find the largest in Y widget
            if (childSize.x > maxWidth)
            {
                maxWidth = childSize.x;
            }
        }
    }

    size.x = maxWidth;

    return size;
}

bool VerticalPanel::refresh()
{
    bool wasInvalidated = invalidated;

    for (const std::unique_ptr<Widget>& childContainer : childContainers)
    {
        if (childContainer)
        {
            if (childContainer->refresh())
            {
                wasInvalidated = true;
            }
        }
    }

    if (wasInvalidated)
    {
        int offset = 0;

        for (const std::unique_ptr<Widget>& childContainer : childContainers)
        {
            childContainer->setVisibility(isVisible());

            if (Widget* child = childContainer->getChild(0))
            {
                childContainer->setSize(child->getGlobalSize());
                childContainer->setPosition(getGlobalPosition() + glm::ivec2(0, offset));

                offset += childContainer->getGlobalSize().y;
            }
        }

        invalidated = false;
    }

    return wasInvalidated;
}

void VerticalPanel::setOffset(int offset)
{
    this->offset = offset;
}
