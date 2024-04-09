#pragma once

#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Event/EventDispatcher.h>

class Entity;
class GizmoComponent;
class EditorContext;
class PointLightComponent;
class Component;

class EditorLocalClient : public LocalClient
{
public:
	EditorLocalClient(Renderer* renderer);

	void onLightCreation(std::shared_ptr<LightComponentCreationEvent> event);

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
