#include "HorizontalPanel.h"

void HorizontalPanel::addChild(Widget* child)
{
    std::unique_ptr<BoxModelPanel> childContainer = std::make_unique<BoxModelPanel>();
    childContainer->addChild(child);
    childContainers.push_back(std::move(childContainer));

    child->invalidate();
    invalidate();
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

    if (invalidated)
    {
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

        invalidated = false;
    }
}