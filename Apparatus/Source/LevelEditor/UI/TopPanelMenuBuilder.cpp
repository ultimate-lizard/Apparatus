#include "TopPanelMenuBuilder.h"

#include <Apparatus/UI/UIContext.h>
#include <Apparatus/UI/Widget/VerticalPanel.h>
#include <Apparatus/UI/Widget/ImagePanel.h>
#include <Apparatus/UI/Widget/Button.h>

void TopPanelMenuBuilder::setContext(UIContext* uiContext)
{
    this->context = uiContext;
}

void TopPanelMenuBuilder::setName(const std::string& name)
{
    this->name = name;
}

void TopPanelMenuBuilder::setTopPanelMenuItem(Button* menuItem)
{
    this->menuItem = menuItem;
}

void TopPanelMenuBuilder::setTopPanel(Widget* topPanel)
{
    this->topPanel = topPanel;
}

void TopPanelMenuBuilder::setParams(const TopPanelMenuParams& params)
{
    this->params = params;
}

Button* TopPanelMenuBuilder::addButton(const std::string& label)
{
    Button* newButton = context->createNinePatchButton(
        "NinePatchButton_" + name + "_" + label,
        params.idleButtonTextureName,
        params.hoverButtonTextureName,
        params.pressButtonTextureName,
        params.buttonBorder,
        label,
        params.buttonFontSize
    );

    newButton->setSize(params.buttonSize);
    newButton->setSizeToContentEnabled(false);

    buttons.push_back(newButton);

    return newButton;
}

Widget* TopPanelMenuBuilder::build()
{
    resultMenu = context->createWidget<Widget>("Widget" + name);
    verticalPanel = context->createWidget<VerticalPanel>("VerticalPanel_" + name);

    resultMenu->addChild(verticalPanel);

    for (Button* button : buttons)
    {
        verticalPanel->addChild(button);
    }

    buttons.clear();

    if (topPanel && menuItem)
    {
        const glm::ivec2 topPanelSize = topPanel->getGlobalSize();
        const glm::ivec2 menuItemPosition = menuItem->getGlobalPosition();
        resultMenu->setPosition({ menuItemPosition.x, topPanelSize.y });
    }

    resultMenu->setVisibility(false);

    return resultMenu;
}
