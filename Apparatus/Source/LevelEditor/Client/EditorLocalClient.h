#pragma once

#include <Apparatus/Client/LocalClient.h>


class EditorLocalClient : public LocalClient
{
public:
	EditorLocalClient(Renderer* renderer, SpriteRenderer* spriteRenderer);

protected:
	virtual void init() override;
	virtual void onGameStart() override;

	virtual void onActiveControllerChanged() override;

	void toggleEditMode();

	void setEditModeEnabled(bool enabled);
	bool isInEditMode() const;

	void setupGlobalEditorInput();

	bool inEditMode;
};
