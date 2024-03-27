#pragma once

#include <Apparatus/Client/LocalClient.h>

class Entity;

enum class InteractionMode
{
	Translation,
	Rotation,
	Scale
};

class EditorLocalClient : public LocalClient
{
public:
	EditorLocalClient(Apparatus* apparatus);

	void setGizmoVisibility(bool enabled);
	void attachGizmo(Entity* parent);

	void selectEntity(Entity* entity);
	Entity* getSelectedEntity();

	InteractionMode getInteractionMode() const;
	void setInteractionMode(InteractionMode interactionMode);

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
	Entity* gizmo;

	InteractionMode interactionMode;
};
