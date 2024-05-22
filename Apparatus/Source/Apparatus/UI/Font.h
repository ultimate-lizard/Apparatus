#pragma once

#include <map>

#include "../Core/Asset.h"
#include "../Rendering/TextureArray.h"

struct Character
{
	int textureArrayDepth = 0;
	glm::ivec2 size = glm::ivec2(0);
	glm::ivec2 bearing = glm::ivec2(0);
	glm::ivec2 advance = glm::ivec2(0);
};

struct CharacterSet
{
	std::map<char, Character> characterMap;
	TextureArray* characterTextureArray;
	unsigned int fontSize = 16;
	unsigned int textureSize = 256;
};

class Font : public Asset
{
public:
	Font();

	Font(const Font&) = delete;
	Font(Font&&) = delete;

	virtual void init() {}

	void addCharacterSet(CharacterSet&& characterSet);
	CharacterSet* getCharacterSet(size_t fontSize);
	CharacterSet* getCurrentCharacterSet();

private:
	// Contains cached character sets of different sizes
	std::map<size_t, CharacterSet> characterSetMap;
	unsigned int fontSize;
};
