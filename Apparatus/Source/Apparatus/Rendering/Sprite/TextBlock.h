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

	void setJustification(TextBlock::Justification justification);
	TextBlock::Justification getJustification() const;

protected:
	float calculateWordLength(const std::string& word);

	std::string text;
	Font* font;
	unsigned int fontSize;

	TextBlock::Justification justification;
};
