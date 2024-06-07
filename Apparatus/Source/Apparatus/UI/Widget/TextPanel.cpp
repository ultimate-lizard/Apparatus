#include "TextPanel.h"

#include "../Font.h"
#include "../../Apparatus.h"
#include "../../Rendering/Material.h"
#include "../../Core/AssetManager/AssetManager.h"

TextPanel::TextPanel()
{
    sizeToContent = true;
    size = { 32, 32 };
}

void TextPanel::init()
{
    if (textBlock = std::make_unique<TextBlock>())
    {
        if (AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>())
        {
            Font* defaultFont = assetManager->findAsset<Font>("Font_Arial");
            textBlock->setFont(defaultFont);
            textBlock->setMaterial(assetManager->findAsset<Material>("Material_TextPanel"));
        }
    }
}

bool TextPanel::refresh()
{
    bool wasInvalidated = Widget::refresh();

    if (wasInvalidated)
    {
        textBlock->setPosition(getGlobalPosition());

        // A small hack to keep the original size instead of content size
        if (sizeToContent)
        {
            sizeToContent = false;
            textBlock->setSize(Widget::getGlobalSize());
            sizeToContent = true;
        }
        else
        {
            textBlock->setSize(Widget::getGlobalSize());
        }

        textBlock->rebuildMesh();

        if (sizeToContent)
        {
            contentSize = textBlock->getDimensions();
        }

        invalidated = false;
    }

    return wasInvalidated;
}

glm::ivec2 TextPanel::getGlobalSize()
{
    if (sizeToContent)
    {
        // A small hack to keep the original size instead of content size
        sizeToContent = false;
        textBlock->setSize(Widget::getGlobalSize());
        sizeToContent = true;

        glm::ivec2 dimensions = textBlock->getDimensions();

        dimensions.x += getMargin(Widget::Side::Left);
        dimensions.x += getMargin(Widget::Side::Right);
        dimensions.y += getMargin(Widget::Side::Top);
        dimensions.y += getMargin(Widget::Side::Bottom);

        return dimensions;
    }

    return Widget::getGlobalSize();
}

void TextPanel::render(SpriteRenderer* renderer)
{
    if (renderer)
    {
        renderer->push(textBlock.get());
    }
}

TextBlock* TextPanel::getTextBlock()
{
    return textBlock.get();
}

void TextPanel::setText(const std::string& text)
{
    textBlock->setText(text);
    invalidate();
}

void TextPanel::setColor(const glm::vec4& color)
{
    textBlock->setColor(color);
    invalidate();
}

void TextPanel::setFontSize(unsigned int fontSize)
{
    textBlock->setFontSize(fontSize);
    invalidate();
}

void TextPanel::setDepth(float depth)
{
    textBlock->setDepth(depth);
    invalidate();
}
