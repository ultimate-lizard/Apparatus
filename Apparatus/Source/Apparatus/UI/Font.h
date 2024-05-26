#pragma once

#include <map>

#include <freetype/freetype.h>

#include "../Core/AssetManager/Asset.h"
#include "../Rendering/TextureArray.h"

struct Character
{
	int textureArrayDepth = 0;
	glm::ivec2 size = glm::ivec2(0);
	glm::ivec2 bearing = glm::ivec2(0);
	glm::ivec2 advance = glm::ivec2(0);
};

struct GlyphCache
{
	std::map<char, Character> glyphMap;
	TextureArray* textureArray;
	unsigned int fontSize = 16;
	unsigned int textureSize = 256;
};

class Font : public Asset
{
public:
	Font(FT_Face face);

	Font(const Font&) = delete;
	Font(Font&&) = delete;

	virtual void init() override;
	virtual void uninit() override;

	GlyphCache* getGlyphCache(unsigned int fontSize);
private:
	GlyphCache* createGlyphCache(unsigned int fontSize);

	FT_Face face;
	std::map<size_t, GlyphCache> glyphCacheMap;
};
