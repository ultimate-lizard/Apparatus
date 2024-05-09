#include "FontImporter.h"

#include "../Rendering/TextureArray.h"
#include "../Apparatus.h"
#include "../UI/Font.h"

void FontImporter::init()
{
	if (FT_Init_FreeType(&ft))
	{
		LOG("Unable to initialize Freetype!", LogLevel::Error);
		return;
	}
}

std::unique_ptr<Font> FontImporter::import(const std::string& fontName, const std::string & path)
{
	// TODO: Those should be variables
	int width = 256;
	int height = 256;
	int layerCount = 128;

	TextureArray* textureArray = Apparatus::getAssetManager().createAsset<TextureArray>("TextureArray_" + fontName, width, height, layerCount, 1);
	std::map<char, Font::Character> characterMap;

	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face))
	{
		LOG("Error loading face!", LogLevel::Error);
		return nullptr;
	}

	FT_Set_Pixel_Sizes(face, width, height);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG("Error loading face!", LogLevel::Error);
			continue;
		}

		textureArray->bind();
		textureArray->copySubData(face->glyph->bitmap.width, face->glyph->bitmap.rows, c, 1, GL_RED, face->glyph->bitmap.buffer);


		Font::Character character;
		character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character.advance = glm::ivec2(face->glyph->advance.x, face->glyph->advance.y);
		character.textureArrayDepth = c;

		characterMap.emplace(c, std::move(character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

    return std::make_unique<Font>(fontName, std::move(characterMap), textureArray);
}
