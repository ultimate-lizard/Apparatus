#include "Font.h"

const std::map<char, Font::Character>& Font::getCharacterMap() const
{
	return characterMap;
}

TextureArray* Font::getTextureArray()
{
	return textureArray;
}

void Font::setFontScale(float fontScale)
{
	this->fontScale = fontScale;
}

float Font::getFontScale() const
{
	return fontScale;
}

Font::Font(const std::string& assetName, std::map<char, Character> characterMap, TextureArray* characterTextureArray) :
	Asset(assetName),
	characterMap(characterMap),
	textureArray(characterTextureArray),
	fontScale(1.0f)
{
}
