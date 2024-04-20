#pragma once

#include <map>

#include <glm/glm.hpp>

#include <Apparatus/Components/Component.h>
#include <Apparatus/Rendering/Rotator.h>

class ModelComponent;
class Window;
class Camera;

enum class InteractionMode
{
	Translation,
	Rotation,
	Scale
};

namespace GizmoNames
{
	static const std::string Gizmo = "Gizmo";
	static const std::string GizmoX = "GizmoX";
	static const std::string GizmoXY = "GizmoXY";
	static const std::string GizmoXZ = "GizmoXZ";
	static const std::string GizmoY = "GizmoY";
	static const std::string GizmoZ = "GizmoZ";
	static const std::string GizmoZY = "GizmoZY";

	static const std::string Pitch = "Pitch";
	static const std::string Yaw = "Yaw";
	static const std::string Roll = "Roll";

	static const std::string Scale = "Scale";
	static const std::string ScaleAll = "ScaleAll";
	static const std::string ScaleUp = "ScaleUp";
	static const std::string ScaleDown = "ScaleDown";
	static const std::string ScaleLeft = "ScaleLeft";
	static const std::string ScaleRight = "ScaleRight";
	static const std::string ScaleFront = "ScaleFront";
	static const std::string ScaleBack = "ScaleBack";
}

struct SnappingData
{
	bool snapToGrid = false;
	Rotator unsnappedRotator;
	glm::vec3 unsnappedScale {0.0f};
	float translationStep = 0.25f;
	float rotationStep = 10.0f;
	float scaleStep = 0.1f;
};

class GizmoComponent : public Component
{
public:
	GizmoComponent();
	GizmoComponent(const std::string& componentName);
	GizmoComponent(const GizmoComponent& other);

	virtual std::unique_ptr<Component> clone() override;

	virtual void init() override;
	virtual void update(float dt) override;

	void setSelectedGizmoModel(const std::string& gizmoModelName);

	InteractionMode getInteractionMode() const;
	void setInteractionMode(InteractionMode interactionMode);

	void setVisibility(bool enabled);
	void attach(Entity* parent);

	void press(const glm::vec3& clickWorldPosition);
	void release();

	bool isPressed() const;

	void handleCursorMovement(float inputX, float inputY);

	void setSnapToGridEnabled(bool enabled);

	void setSelectedEntity(Entity* selectedEntity);

protected:
	void updateVisibility();

	void handleTranslation(Window& window, Camera* camera);
	void handleRotation(Window& window, Camera* camera);
	void handleScale(Window& window, Camera* camera);

	glm::vec3 getLocalUpForGimbal(const std::string& gimbalName) const;
	Euler getEulerAngleOfGimbal(const std::string& gimbalName) const;
	glm::vec2 projectOnPlane(const glm::vec3& position, const glm::vec3& normal) const;

private:
	InteractionMode interactionMode;

	ModelComponent* selectedGizmoModel;

	bool pressed;
	bool visible;

	float rotationClickAngle;
	glm::vec3 rotationClickPosition;
	glm::vec3 gizmoSelectPositionOrigin;
	glm::vec3 gizmoClickPosition;
	glm::vec3 gizmoClickOffset;
	glm::ivec2 lastCursorScreenPosition;
	glm::vec2 clickCursorDevicePosition;

	Entity* selectedEntity;

	// This is a rotation value used during rotation manipulation. A rounded version is used to set rotation when grid enabled
	SnappingData snappingData;
};
