#include "HorizontalPanel.h"

void HorizontalPanel::addChild(Widget* child)
{
    std::unique_ptr<Widget> childContainer = std::make_unique<Widget>();
    childContainer->addChild(child);
    childContainers.push_back(std::move(childContainer));
    child->invalidate();
    invalidate();

    // TODO: Unparent the child first!!!
    // children.push_back(child);
}

glm::ivec2 HorizontalPanel::getGlobalSize()
{
    glm::ivec2 size(0);
    int maxHeight = 0;

    for (const std::unique_ptr<Widget>& childContainer : childContainers)
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

bool HorizontalPanel::refresh()
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
        float offset = 0.0f;

        for (const std::unique_ptr<Widget>& childContainer : childContainers)
        {
            if (Widget* child = childContainer->getChild(0))
            {
                childContainer->setSize(child->getGlobalSize());
                childContainer->setPosition(getGlobalPosition() + glm::ivec2(offset, 0));

                offset += childContainer->getGlobalSize().x;
            }
        }

        invalidated = false;
    }

    return wasInvalidated;
}