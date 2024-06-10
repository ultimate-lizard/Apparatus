#pragma once

#include <string>

#include "Drawable.h"

class Font;

class TextBlock : public Drawable
{
public:
	enum class Justification
	{
		Left,
		Center,
		Right
	};

	TextBlock();
	virtual ~TextBlock() = default;

	virtual void rebuildMesh() override;

	void setText(const std::string& text);
	const std::string& getString() const;

	void setFont(Font* font);
	Font* getFont();

	void setFontSize(unsigned int fontScale);
	unsigned int getFontSize();

	glm::ivec2 getDimensions();

protected:
	float calculateWordLength(const std::string& word) const;

	std::string text;
	std::string cachedText;

	Font* font;
	unsigned int fontSize;

	glm::ivec2 cachedDimensions;
};
