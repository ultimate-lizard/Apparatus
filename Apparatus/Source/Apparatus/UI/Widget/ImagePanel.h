#pragma once

#include "BoxModelPanel.h"

#include "../../Rendering/Sprite/Sprite.h"

class ImagePanel : public BoxModelPanel
{
public:
	virtual void init() override;
	virtual void refresh() override;

	virtual void render(SpriteRenderer* renderer) override;

	Sprite* getSprite();
	void setTexture(Texture* texture);

	void setDepth(float depth);
	float getDepth() const;

protected:
	std::unique_ptr<Sprite> sprite;
};
