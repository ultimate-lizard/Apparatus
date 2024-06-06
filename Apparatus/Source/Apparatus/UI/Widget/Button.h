#pragma once

#include "BoxModelPanel.h"

class ImagePanel;
class TextPanel;
class Sprite;
class Texture;

class Button : public BoxModelPanel
{
public:
	enum class ButtonState
	{
		Idle,
		Hover,
		Press
	};

	Button();

	virtual void init() override;

	// void addPanelForState(ImagePanel* idleState, ButtonState state);
	// void addLabel(TextPanel* textPanel);

	//virtual glm::ivec2 getGlobalSize() const override;
	virtual bool refresh() override;

	virtual void render(SpriteRenderer* renderer) override;

	virtual void onMouseEnter() override;
	virtual void onMouseLeave() override;
	virtual bool onKeyInput(InputKey key, KeyEventType type) override;

	void setIdleTexture(Texture* texture);
	void setHoverTexture(Texture* texture);
	void setPressTexture(Texture* texture);

	void setButtonState(ButtonState state);
	void setLabelClickOffset(int offset);

protected:
	virtual void initMaterial();
	virtual void refreshSprite();
	
	std::unique_ptr<Sprite> idleSprite;
	std::unique_ptr<Sprite> hoverSprite;
	std::unique_ptr<Sprite> pressSprite;
	Sprite* currentSprite;
	
	//ImagePanel* idleStatePanel;
	//ImagePanel* hoverStatePanel;
	//ImagePanel* pressStatePanel;
	//ImagePanel* currentStatePanel;

	// TextPanel* label;

	ButtonState buttonState;

	bool hovered;

	int childClickOffset;
};
