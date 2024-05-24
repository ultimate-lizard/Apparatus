#include "FontImporter.h"

#include "../Rendering/TextureArray.h"
#include "../Apparatus.h"
#include "../UI/Font.h"
#include "Logger.h"

void FontImporter::init()
{
	if (FT_Init_FreeType(&ft))
	{
		LOG("Unable to initialize Freetype!", LogLevel::Error);
		return;
	}
}

void FontImporter::uninit()
{
	FT_Done_FreeType(ft);
}

std::unique_ptr<Font> FontImporter::import(const std::string & path)
{
	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face))
	{
		LOG("Error loading face!", LogLevel::Error);
		return nullptr;
	}

	auto font = std::make_unique<Font>(face);

	// font->create

    return font;
}
