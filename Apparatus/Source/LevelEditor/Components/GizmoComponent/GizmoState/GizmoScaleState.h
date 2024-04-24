#pragma once

#include "GizmoState.h"

class GizmoScaleState : public GizmoState
{
public:
	GizmoScaleState(SnappingData& snappingData);

	virtual void updateVisibility() override;
	virtual void handleGizmoClick(const glm::vec3& clickWorldPosition) override;
	virtual void handleGizmoRelease() override;
	virtual void handleCursorMovement(const glm::ivec2& cursorPosition, Camera* camera, TransformComponent* selectedEntityTransformComponent) override;

protected:
	glm::vec3 clickPosition;
	glm::vec3 positionDuringClick;
	glm::vec2 lastCursorDevicePosition;
	glm::ivec2 lastCursorPosition;
};
