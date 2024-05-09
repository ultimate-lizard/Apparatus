#pragma once

#include <string>

#include "Drawable.h"

class Font;

class TextBlock : public Drawable
{
public:
	TextBlock(int bufferSize);
	virtual ~TextBlock() = default;

	virtual void rebuildMesh() override;

	void setText(const std::string& text);
	const std::string& getString() const;

	void setFont(Font* font);
	Font* getFont();

	void setFontSize(float fontScale);
	float getFontScale();

protected:
	float calculateWordLength(const std::string& word);

	std::string text;
	Font* font;
};
