#pragma once

#include <Apparatus/Client/LocalClient.h>

class Entity;
class GizmoComponent;

class EditorLocalClient : public LocalClient
{
public:
	EditorLocalClient(Apparatus* apparatus);

	void selectEntity(Entity* entity);
	Entity* getSelectedEntity();

	GizmoComponent* getGizmo();

protected:
	virtual void assignDefaultObjectName() override;

	virtual void init() override;
	virtual void start() override;

	virtual void onActiveControllerChanged() override;

	void indicateSelection(Entity* entity, bool selected);

	void toggleEditMode();

	void setEditModeEnabled(bool enabled);
	bool isInEditMode() const;

	void setupGlobalEditorInput();

	bool inEditMode;
	Entity* selectedEntity;
	GizmoComponent* gizmo;
};
