#include "TextPanel.h"

#include "../Font.h"
#include "../../Apparatus.h"
#include "../../Rendering/Material.h"
#include "../../Core/AssetManager/AssetManager.h"

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

    if (wasInvalidated && textBlock)
    {
        textBlock->setPosition(getGlobalPosition());
        // Size is boundaries of the text. If 0, no boundaries are set.
        // Global size is the actual size of the text
        textBlock->setSize(getSize());
        textBlock->rebuildMesh();

        invalidated = false;
    }

    if (sizeToContent)
    {
        contentSize = textBlock->getDimensions();
    }

    return wasInvalidated;
}

//glm::ivec2 TextPanel::getGlobalSize() const
//{
//    if (sizeToContent)
//    {
//        if (horizontalAlignment != Alignment::Fill && verticalAlignment != Alignment::Fill)
//        {
//            dimensions.x += getMargin(Widget::Side::Left);
//            dimensions.x += getMargin(Widget::Side::Right);
//            dimensions.y += getMargin(Widget::Side::Top);
//            dimensions.y += getMargin(Widget::Side::Bottom);
//        }
//    }
//
//    return dimensions;
//}

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
