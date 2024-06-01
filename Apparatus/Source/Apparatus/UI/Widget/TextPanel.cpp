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

void TextPanel::refresh()
{
    Widget::refresh();

    if (invalidated && textBlock)
    {
        textBlock->setPosition(getGlobalPosition());
        textBlock->setSize(getGlobalSize());
        textBlock->rebuildMesh();

        invalidated = false;
    }
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
    if (textBlock)
    {
        textBlock->setText(text);
        invalidate();
    }
}

void TextPanel::setColor(const glm::vec4& color)
{
    if (textBlock)
    {
        textBlock->setColor(color);
        invalidate();
    }
}

void TextPanel::setFontSize(unsigned int fontSize)
{
    if (textBlock)
    {
        textBlock->setFontSize(fontSize);
        invalidate();
    }
}

void TextPanel::setDepth(float depth)
{
    textBlock->setDepth(depth);
    invalidate();
}
