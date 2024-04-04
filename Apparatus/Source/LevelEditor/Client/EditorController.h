#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Apparatus/Client/HumanControllerBase.h>
#include <Apparatus/Rendering/Rotator.h>

class InputHandler;
class EditorLocalClient;
class ModelComponent;
class GizmoComponent;
struct RayTraceResult;

class EditorController : public HumanControllerBase
{
public:
	EditorController(EditorLocalClient* editorLocalClient);

	virtual void onActivate() override;
	virtual void onDeactivate() override;

protected:
	virtual void assignDefaultObjectName() override;

	virtual void setupInput() override;

	void pressSelect();
	void releaseSelect();

	void enableTranslationMode();
	void enableRotationMode();
	void enableScaleMode();

	void releasePrimaryMouseButton();

	void cursorMoveY(float value);
	void cursorMoveX(float value);

	EditorLocalClient* editorLocalClient;
	GizmoComponent* gizmo;
};
