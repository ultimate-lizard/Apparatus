#pragma once

#include <Apparatus/Server/LocalServer.h>

class GizmoComponent;
class TransformComponent;

class EditorLocalServer : public LocalServer
{
public:
	EditorLocalServer();

	virtual void connect(const ConnectionInfo& info) override;

	virtual void update(float dt) override;
	virtual void start() override;

	void selectEntity(Entity* entity);
	
	void duplicateSelection();

	void setSnapToGridEnabled(bool enabled);
	bool isSnapToGridEnabled();

	void setSelectionBoxVisibility(bool visible);
	bool isSelectionBoxVisible();

	void regenerateSelectionBoundingBox();

	void setShiftPressed(bool pressed);
	bool isShiftPressed() const;

protected:
	void indicateSelection(Entity* entity, bool selected);

	// Entity* selectedEntity;
	std::vector<Entity*> selectedEntities;

	GizmoComponent* gizmo;
	TransformComponent* selectionProxyTransformComponent;

	bool snapToGrid;
	bool shiftPressed;
};
