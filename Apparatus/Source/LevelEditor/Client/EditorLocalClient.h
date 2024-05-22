#pragma once

#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Rendering/Sprite/TextBlock.h>
#include <Apparatus/UI/Widget/NinePatchPanel.h>

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
};
