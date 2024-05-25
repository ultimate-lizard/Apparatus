#include "Font.h"

#include <glad/glad.h>

#include "../Apparatus.h"
#include "../Core/AssetManager.h"
#include "../Rendering/TextureArray.h"

void Font::createGlyphCache(unsigned int fontSize)
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	if (!assetManager)
	{
		return;
	}

	if (characterSetMap.find(fontSize) != characterSetMap.end())
	{
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Get default size

	// TODO: Those should be variables
	int width = 14;
	int height = 14;
	int layerCount = 128;

	TextureArray* textureArray = assetManager->createAsset<TextureArray>("TextureArray_DummyFontName", width, height, layerCount, 1);
	std::map<char, Character> characterMap;

	FT_Set_Pixel_Sizes(face, width, height);

	for (unsigned char c = 0; c < 128; ++c)
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
	glyphCache.fontSize = 14;
	glyphCache.textureSize = height;

	characterSetMap[glyphCache.fontSize] = std::move(glyphCache);
}

void Font::init()
{
	createGlyphCache(14);
}

void Font::uninit()
{
	FT_Done_Face(face);
}

GlyphCache* Font::getCharacterSet(size_t fontSize)
{
	auto setSearchIter = characterSetMap.find(fontSize);
	if (setSearchIter != characterSetMap.cend())
	{
		return &setSearchIter->second;
	}

	return nullptr;
}

GlyphCache* Font::getCurrentCharacterSet()
{
	return getCharacterSet(fontSize);
}

Font::Font(FT_Face face) :
	face(face),
	fontSize(14)
{

}
