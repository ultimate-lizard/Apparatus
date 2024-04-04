#pragma once

#include <map>

#include <glm/glm.hpp>

#include <Apparatus/Components/Component.h>
#include <Apparatus/Rendering/Rotator.h>

class ModelComponent;
class TransformComponent;
class EditorLocalClient;

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

class GizmoComponent : public Component
{
public:
	GizmoComponent(Entity* owner);
	GizmoComponent(Entity* owner, const std::string& name);
	~GizmoComponent() = default;

	virtual void init() override;
	virtual void update(float dt) override;

	void setEditorLocalClient(EditorLocalClient* editorLocalClient);
	void setSelectedGizmoModel(ModelComponent* modelComponent);

	InteractionMode getInteractionMode() const;
	void setInteractionMode(InteractionMode interactionMode);

	void setVisibility(bool enabled);
	void attach(Entity* parent);

	void press(const glm::vec3& clickWorldPosition);
	void release();

	bool isPressed() const;

	void handleCursorMovement(float inputX, float inputY);

protected:
	virtual void assignDefaultObjectName();

	void updateVisibility();

	void handleTranslation();
	void handleRotation();
	void handleScale();

	glm::vec3 getLocalUpForGimbal(const std::string& gimbalName) const;
	Euler getEulerAngleOfGimbal(const std::string& gimbalName) const;
	glm::vec2 projectOnPlane(const glm::vec3& position, const glm::vec3& normal) const;

private:
	EditorLocalClient* editorLocalClient;

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
};
