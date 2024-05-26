#include "Font.h"

#include <glad/glad.h>

#include "../Apparatus.h"
#include "../Core/AssetManager.h"
#include "../Rendering/TextureArray.h"

GlyphCache* Font::createGlyphCache(unsigned int fontSize)
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	if (!assetManager)
	{
		return nullptr;
	}

	auto glyphSearchIter = glyphCacheMap.find(fontSize);
	if (glyphSearchIter != glyphCacheMap.end())
	{
		return &glyphSearchIter->second;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// TODO: Implement UTF-8
	const unsigned int characterAmount = 128;

	int width = fontSize;
	int height = fontSize;
	int layerCount = characterAmount;

	std::string textureName = "TextureArray_";
	textureName += getAssetName() + "_" + std::to_string(fontSize);

	TextureArray* textureArray = assetManager->createAsset<TextureArray>(textureName, width, height, layerCount, 1);
	std::map<char, Character> characterMap;

	FT_Set_Pixel_Sizes(face, width, height);

	for (unsigned char c = 0; c < characterAmount; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG("Error loading face!", LogLevel::Error);
			continue;
		}

		textureArray->bind();
		textureArray->copySubData(face->glyph->bitmap.width, face->glyph->bitmap.rows, c, 1, GL_RED, face->glyph->bitmap.buffer);

		Character character;
		character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character.advance = glm::ivec2(face->glyph->advance.x, face->glyph->advance.y);
		character.textureArrayDepth = c;

		characterMap.emplace(c, std::move(character));
	}

	GlyphCache glyphCache;
	glyphCache.glyphMap = std::move(characterMap);
	glyphCache.textureArray = textureArray;
	glyphCache.fontSize = fontSize;
	glyphCache.textureSize = height;

	glyphCacheMap[glyphCache.fontSize] = std::move(glyphCache);

	return &glyphCacheMap[fontSize];
}

void Font::init()
{
	createGlyphCache(14);
}

void Font::uninit()
{
	FT_Done_Face(face);
}

GlyphCache* Font::getGlyphCache(unsigned int fontSize)
{
	auto setSearchIter = glyphCacheMap.find(fontSize);
	if (setSearchIter != glyphCacheMap.cend())
	{
		return &setSearchIter->second;
	}

	return createGlyphCache(fontSize);
}

//GlyphCache* Font::getCurrentGlyphCache()
//{
//	return getGlyphCache(fontSize);
//}

Font::Font(FT_Face face) :
	face(face)
{

}
