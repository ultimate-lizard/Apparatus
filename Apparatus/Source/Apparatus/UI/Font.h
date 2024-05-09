#pragma once

#include <map>

#include "../Core/Asset.h"
#include "../Rendering/TextureArray.h"

class Font : public Asset
{
public:
	struct Character
	{
		int textureArrayDepth = 0;
		glm::ivec2 size = glm::ivec2(0);
		glm::ivec2 bearing = glm::ivec2(0);
		glm::ivec2 advance = glm::ivec2(0);
	};

	Font(const std::string& assetName, std::map<char, Font::Character> characterMap, TextureArray* characterTextureArray);

	Font(const Font&) = delete;
	Font(Font&&) = delete;

	virtual void init() {}

	const std::map<char, Font::Character>& getCharacterMap() const;
	TextureArray* getTextureArray();

	void setFontScale(float fontScale);
	float getFontScale() const;

private:
	std::map<char, Font::Character> characterMap;
	TextureArray* textureArray;
	float fontScale;
};
