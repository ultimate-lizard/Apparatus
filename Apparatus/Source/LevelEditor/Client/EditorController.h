#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Apparatus/Client/HumanControllerBase.h>
#include <Apparatus/Rendering/Rotator.h>

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

	void toggleDebugPrimitives();

	void enableTranslationMode();
	void enableRotationMode();

	void releasePrimaryMouseButton();

	void cursorMoveY(float value);
	void cursorMoveX(float value);
	void cursorMove(float mouseInputX, float mouseInputY);

	void handleGizmoTranslation();
	void handleGizmoRotation(float mouseInputX, float mouseInputY);

	float getPositionAngle(const glm::vec3& center, const glm::vec2& position);

	Euler getSelectedGimbalEulerAngle();
	glm::vec3 getSelectedGimbalUp();
	glm::vec2 positionRelativeToGimbal(const glm::vec3& position);

	EditorLocalClient* editorLocalClient;

	bool gizmoPressed;
	ModelComponent* selectedGizmoModel;
	glm::vec3 gizmoSelectPositionOrigin;
	glm::vec3 clickOffset;
};
