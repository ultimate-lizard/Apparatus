#pragma once

#include "BoxModelPanel.h"

#include "../../Rendering/Sprite/TextBlock.h"

class TextPanel : public BoxModelPanel
{
public:
	virtual void init() override;
	virtual void refresh() override;

	// virtual glm::ivec2 getGlobalSize() const override;

	virtual void render(SpriteRenderer* renderer) override;

	TextBlock* getTextBlock();
	void setText(const std::string& text);
	void setColor(const glm::vec4& color);
	void setFontSize(unsigned int fontSize);

	void setDepth(float depth);

protected:
	std::unique_ptr<TextBlock> textBlock;
};
