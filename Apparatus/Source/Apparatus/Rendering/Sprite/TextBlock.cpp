#include "TextBlock.h"

#include <sstream>

#include "../Material.h"
#include "../../UI/Font.h"

static const int textBufferSize = 4000 * 6 * 5 * sizeof(float); // chars * vertices * attribute * float

TextBlock::TextBlock() :
    font(nullptr)
{
    auto vao = std::make_unique<VertexArrayObject>();
    assert(vao);
    vao->setStride(5 * sizeof(float));
    vao->addAttribute(2);
    vao->addAttribute(3);

    spriteMesh = std::make_unique<Mesh>(std::move(vao), textBufferSize, 0);
    assert(spriteMesh);
    spriteMesh->init();
}

void TextBlock::rebuildMesh()
{
    std::vector<float> textVertices;

    if (!font)
    {
        return;
    }

    GlyphCache* currentCharacterSet = font->getCurrentCharacterSet();
    if (!currentCharacterSet)
    {
        return;
    }

    if (material)
    {
        MaterialParameters& params = material->getParameters();
        params.setTextureArray("textTextureArray", currentCharacterSet->textureArray);
    }

    float x = static_cast<float>(getPosition().x);
    float y = static_cast<float>(getPosition().y);
    float xstart = x;

    float fontScale = static_cast<float>(currentCharacterSet->fontSize) / currentCharacterSet->textureSize;

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
                y += currentCharacterSet->textureSize * fontScale;
                x = xstart;
            }

            if (size.y != 0 && y > size.y + position.y)
            {
                continue;
            }

            for (auto iter = word.begin(); iter != word.end(); ++iter)
            {
                const auto characterMap = currentCharacterSet->glyphMap;
                auto searchIter = characterMap.find(*iter);
                if (searchIter == characterMap.end())
                {
                    continue;
                }

                const Character& character = searchIter->second;

                if (*iter == '\n')
                {
                    y += currentCharacterSet->textureSize * fontScale;
                    x = xstart;
                    continue;
                }

                const float textureMinX = 0.0f;
                const float textureMinY = 0.0f;
                const float textureMaxX = 1.0f;
                const float textureMaxY = 1.0f;

                // TODO: get size properly
                glm::vec2 glyphSize(currentCharacterSet->textureSize * fontScale);

                float positionX = x + character.bearing.x * fontScale;
                float positionY = y + (currentCharacterSet->textureSize - character.bearing.y) * fontScale;
                glm::vec2 glyphPosition(positionX, positionY);

                const std::vector<float> glyphVertices = {
                    1.0f * glyphSize.x + glyphPosition.x, 0.0f * glyphSize.y + glyphPosition.y, textureMaxX, textureMinY, static_cast<float>(character.textureArrayDepth),
                    0.0f * glyphSize.x + glyphPosition.x, 0.0f * glyphSize.y + glyphPosition.y, textureMinX, textureMinY, static_cast<float>(character.textureArrayDepth),
                    0.0f * glyphSize.x + glyphPosition.x, 1.0f * glyphSize.y + glyphPosition.y, textureMinX, textureMaxY, static_cast<float>(character.textureArrayDepth),
                    1.0f * glyphSize.x + glyphPosition.x, 0.0f * glyphSize.y + glyphPosition.y, textureMaxX, textureMinY, static_cast<float>(character.textureArrayDepth),
                    0.0f * glyphSize.x + glyphPosition.x, 1.0f * glyphSize.y + glyphPosition.y, textureMinX, textureMaxY, static_cast<float>(character.textureArrayDepth),
                    1.0f * glyphSize.x + glyphPosition.x, 1.0f * glyphSize.y + glyphPosition.y, textureMaxX, textureMaxY, static_cast<float>(character.textureArrayDepth),
                };

                textVertices.insert(textVertices.end(), glyphVertices.begin(), glyphVertices.end());

                x += (character.advance.x >> 6) * fontScale;
            }
        }
    }

    getSpriteMesh()->bind();

    auto vertexBuffer = std::make_shared<VertexBuffer<float>>();
    assert(vertexBuffer);
    vertexBuffer->vertices = textVertices;
    getSpriteMesh()->setSubData(vertexBuffer);
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
        //font->setFontSize(fontScale);
    }
}

float TextBlock::getFontScale()
{
    if (font)
    {
        //return font->getFontScale();
    }

    return 0.0f;
}

float TextBlock::calculateWordLength(const std::string& word)
{
    float result = 0.0f;

    GlyphCache* currentCharacterSet = font->getCurrentCharacterSet();
    if (!currentCharacterSet)
    {
        return 0.0;
    }

    if (font)
    {
        for (auto iter = word.begin(); iter != word.end(); ++iter)
        {
            const auto characterMap = currentCharacterSet->glyphMap;
            auto searchIter = characterMap.find(*iter);
            if (searchIter == characterMap.end())
            {
                continue;
            }

            const Character& character = searchIter->second;
            result += (character.advance.x >> 6) * currentCharacterSet->fontSize / currentCharacterSet->textureSize;
        }
    }

    return result;
}
