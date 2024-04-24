#pragma once

#include <string>

#include <Apparatus/Rendering/Rotator.h>

#include "GizmoState.h"

class GizmoRotationState : public GizmoState
{
public:
	GizmoRotationState(SnappingData& snappingData);

	virtual void handleGizmoClick(const glm::vec3& clickWorldPosition) override;
	virtual void handleGizmoRelease() override;
	virtual void handleCursorMovement(const glm::ivec2& cursorPosition, Camera* camera, TransformComponent* selectedEntityTransformComponent) override;

protected:
	virtual void updateVisibility() override;

	glm::vec3 getLocalUpForGimbal(const std::string& gimbalName) const;
	glm::vec2 projectOnPlane(const glm::vec3& position, const glm::vec3& normal) const;
	Euler getEulerAngleOfGimbal(const std::string& gimbalName) const;

	glm::vec3 clickPosition;
	float clickAngle;
	glm::ivec2 lastCursorPosition;
};
