#include "TextPanel.h"

#include "../Font.h"
#include "../../Apparatus.h"
#include "../../Rendering/Material.h"

void TextPanel::init()
{
    if (textBlock = std::make_unique<TextBlock>())
    {
        Font* defaultFont = Apparatus::getAssetManager().findAsset<Font>("Font_Arial");
        textBlock->setFont(defaultFont);
        textBlock->setMaterial(Apparatus::getAssetManager().findAsset<Material>("Material_TextPanel"));
    }
}

void TextPanel::refresh()
{
    Widget::refresh();

    if (textBlock)
    {
        textBlock->setPosition(getGlobalPosition());
        textBlock->setSize(getGlobalSize());
        textBlock->rebuildMesh();
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
    }
}

void TextPanel::setColor(const glm::vec4& color)
{
    if (textBlock)
    {
        textBlock->setColor(color);
    }
}

void TextPanel::setFontSize(float fontSize)
{
    if (textBlock)
    {
        textBlock->setFontSize(fontSize);
    }
}

void TextPanel::setDepth(float depth)
{
    textBlock->setDepth(depth);
}
