#include "TextBlock.h"

#include <sstream>

#include "../Material.h"
#include "../../UI/Font.h"

TextBlock::TextBlock(int bufferSize) :
    Drawable(bufferSize),
    font(nullptr)
{

}

void TextBlock::rebuildMesh()
{
    std::vector<float> textVertices;

    if (!font)
    {
        return;
    }

    if (material)
    {
        MaterialParameters& params = material->getParameters();
        params.setTextureArray("textTextureArray", font->getTextureArray());
    }

    float x = static_cast<float>(getPosition().x);
    float y = static_cast<float>(getPosition().y);
    float xstart = x;

    float fontScale = font->getFontScale() / 256.0f;

    // Iterate over each line
    std::stringstream textStream(text);
    for (std::string line; std::getline(textStream, line);)
    {
        // Iterate over each word
        std::stringstream lineStream(line);
        while (lineStream)
        {
            std::string word;
            lineStream >> word;

            // Add symbols that were omitted by the stream iteration
            if (lineStream.peek() == std::char_traits<char>::eof())
            {
                word += '\n';
            }
            else
            {
                word += ' ';
            }

            // Constraint the text in its size dimensions
            const float wordLength = calculateWordLength(word);
            if (size.x != 0 && wordLength + x >= size.x + position.x)
            {
                y += 256 * fontScale;
                x = xstart;
            }

            if (size.y != 0 && y > size.y + position.y)
            {
                continue;
            }

            for (auto iter = word.begin(); iter != word.end(); ++iter)
            {
                const auto characterMap = font->getCharacterMap();
                auto searchIter = characterMap.find(*iter);
                if (searchIter == characterMap.end())
                {
                    continue;
                }

                const Font::Character& character = searchIter->second;

                if (*iter == '\n')
                {
                    y += 256 * fontScale;
                    x = xstart;
                    continue;
                }

                const float textureMinX = 0.0f;
                const float textureMinY = 0.0f;
                const float textureMaxX = 1.0f;
                const float textureMaxY = 1.0f;

                // TODO: get size properly
                glm::vec2 glyphSize(256 * fontScale);

                float positionX = x + character.bearing.x * fontScale;
                float positionY = y + (256.0f - character.bearing.y) * fontScale;
                glm::vec2 glyphPosition(positionX, positionY);

                // float glyphDepth = getDepth();

                const std::vector<float> glyphVertices = {
                    1.0f * glyphSize.x + glyphPosition.x, 0.0f * glyphSize.y + glyphPosition.y, static_cast<float>(character.textureArrayDepth), textureMaxX, textureMinY,
                    0.0f * glyphSize.x + glyphPosition.x, 0.0f * glyphSize.y + glyphPosition.y, static_cast<float>(character.textureArrayDepth), textureMinX, textureMinY,
                    0.0f * glyphSize.x + glyphPosition.x, 1.0f * glyphSize.y + glyphPosition.y, static_cast<float>(character.textureArrayDepth), textureMinX, textureMaxY,
                    1.0f * glyphSize.x + glyphPosition.x, 0.0f * glyphSize.y + glyphPosition.y, static_cast<float>(character.textureArrayDepth), textureMaxX, textureMinY,
                    0.0f * glyphSize.x + glyphPosition.x, 1.0f * glyphSize.y + glyphPosition.y, static_cast<float>(character.textureArrayDepth), textureMinX, textureMaxY,
                    1.0f * glyphSize.x + glyphPosition.x, 1.0f * glyphSize.y + glyphPosition.y, static_cast<float>(character.textureArrayDepth), textureMaxX, textureMaxY,
                };

                textVertices.insert(textVertices.end(), glyphVertices.begin(), glyphVertices.end());

                x += (character.advance.x >> 6) * fontScale;
            }
        }
    }

    getSpriteMesh()->bind();
    getSpriteMesh()->setSubData(textVertices);
}

void TextBlock::setText(const std::string& text)
{
    this->text = text;
}

const std::string& TextBlock::getString() const
{
    return text;
}

void TextBlock::setFont(Font* font)
{
    this->font = font;
}

Font* TextBlock::getFont()
{
    return font;
}

void TextBlock::setFontSize(float fontScale)
{
    if (font)
    {
        font->setFontScale(fontScale);
    }
}

float TextBlock::getFontScale()
{
    if (font)
    {
        return font->getFontScale();
    }

    return 0.0f;
}

float TextBlock::calculateWordLength(const std::string& word)
{
    float result = 0.0f;

    if (font)
    {
        for (auto iter = word.begin(); iter != word.end(); ++iter)
        {
            const auto characterMap = font->getCharacterMap();
            auto searchIter = characterMap.find(*iter);
            if (searchIter == characterMap.end())
            {
                continue;
            }

            const Font::Character& character = searchIter->second;
            result += (character.advance.x >> 6) * font->getFontScale() / 256.0f;
        }
    }

    return result;
}
