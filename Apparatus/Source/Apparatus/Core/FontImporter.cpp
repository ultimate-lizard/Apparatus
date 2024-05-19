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
	auto font = std::make_unique<Font>(fontName);


	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face))
	{
		LOG("Error loading face!", LogLevel::Error);
		return nullptr;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Get default size

	// TODO: Those should be variables
 	int width = 14;
	int height = 14;
	int layerCount = 128;

	TextureArray* textureArray = Apparatus::getAssetManager().createAsset<TextureArray>("TextureArray_" + fontName, width, height, layerCount, 1);
	std::map<char, Character> characterMap;

	FT_Set_Pixel_Sizes(face, width, height);

	for (unsigned char c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG("Error loading face!", LogLevel::Error);
			continue;
		}

		// FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		textureArray->bind();
		textureArray->copySubData(face->glyph->bitmap.width, face->glyph->bitmap.rows, c, 1, GL_RED, face->glyph->bitmap.buffer);

		Character character;
		character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character.advance = glm::ivec2(face->glyph->advance.x, face->glyph->advance.y);
		character.textureArrayDepth = c;

		characterMap.emplace(c, std::move(character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	CharacterSet set;
	set.characterMap = std::move(characterMap);
	set.characterTextureArray = textureArray;
	set.fontSize = 14;
	set.textureSize = height;
	font->addCharacterSet(std::move(set));

    return font;
}
