#pragma once

#include "Widget.h"

#include "../../Rendering/Sprite/TextBlock.h"

class TextPanel : public Widget
{
public:
	TextPanel();

	virtual void init() override;
	virtual bool refresh() override;

	// Returns the actual size of the text
	virtual glm::ivec2 getGlobalSize();

	virtual void render(SpriteRenderer* renderer) override;

	TextBlock* getTextBlock();
	void setText(const std::string& text);
	void setColor(const glm::vec4& color);
	void setFontSize(unsigned int fontSize);

	void setDepth(float depth);

protected:
	std::unique_ptr<TextBlock> textBlock;
};
