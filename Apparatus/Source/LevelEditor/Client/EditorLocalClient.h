#pragma once

#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Rendering/Sprite/TextBlock.h>
#include <Apparatus/UI/Panel/NinePatchPanel.h>

class EditorLocalClient : public LocalClient
{
public:
	EditorLocalClient(Renderer* renderer, SpriteRenderer* spriteRenderer);

protected:
	virtual void init() override;
	virtual void update(float dt) override;
	virtual void onGameStart() override;

	virtual void onActiveControllerChanged() override;

	void toggleEditMode();

	void setEditModeEnabled(bool enabled);
	bool isInEditMode() const;

	void setupGlobalEditorInput();

	bool inEditMode;

	std::unique_ptr<TextBlock> testTextBlock;
	// std::unique_ptr<TextPanel> textPanel;
	std::unique_ptr<Sprite> thickWindowSprite;
	std::unique_ptr<Sprite> thinWindowSprite;
	std::unique_ptr<Sprite> anotherThickSprite;
	// std::unique_ptr<NinePatchPanel> testPanel;
	std::vector<std::unique_ptr<Widget>> testPanels;
};
