#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Apparatus/Client/HumanControllerBase.h>

class InputHandler;
class EditorLocalClient;
class ModelComponent;
struct RayTraceResult;

class EditorController : public HumanControllerBase
{
public:
	EditorController(EditorLocalClient* editorLocalClient);

	virtual void onActivate() override;
	virtual void onDeactivate() override {};
	
protected:
	virtual void assignDefaultObjectName() override;

	virtual void setupInput() override;

	void pressSelect();
	void releaseSelect();

	void enableTranslationMode();
	void enableRotationMode();

	void releasePrimaryMouseButton();

	void cursorMoveY(float value);
	void cursorMoveX(float value);
	void cursorMove(float mouseInputX, float mouseInputY);

	void handleGizmoTranslation();
	void handleGizmoRotation(float mouseInputX, float mouseInputY);

	EditorLocalClient* editorLocalClient;

	bool gizmoPressed;
	ModelComponent* selectedGizmoModel;
	glm::vec3 gizmoSelectOrigin;
	glm::vec3 clickOffset;
};
