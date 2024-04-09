#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Apparatus/Client/HumanControllerBase.h>
#include <Apparatus/Rendering/Rotator.h>

class GizmoComponent;

class EditorController : public HumanControllerBase
{
public:
	EditorController(const std::string& controllerName, LocalClient& localClient);

	virtual void onActivate() override;
	virtual void onDeactivate() override;

protected:
	virtual void setupInput() override;

	void pressSelect();
	void cancelSelection();

	void enableTranslationMode();
	void enableRotationMode();
	void enableScaleMode();

	void releaseSelect();

	void cursorMoveY(float value);
	void cursorMoveX(float value);

	void onModifierPressed();
	void onModifierReleased();
	void onDuplicate();

	GizmoComponent* gizmo;
	bool ctrlPressed;
};
