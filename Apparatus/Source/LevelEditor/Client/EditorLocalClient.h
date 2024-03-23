#pragma once

#include <Apparatus/Client/LocalClient.h>

class Entity;

class EditorLocalClient : public LocalClient
{
public:
	EditorLocalClient(Apparatus* apparatus);

protected:
	virtual void assignDefaultObjectName() override;

	virtual void init() override;

	virtual void onActiveControllerChanged() override;

	void toggleEditMode();

	void setEditModeEnabled(bool enabled);
	bool isInEditMode() const;

	void setupGlobalEditorInput();

	Entity* selectedEntity;
	bool inEditMode;
};
