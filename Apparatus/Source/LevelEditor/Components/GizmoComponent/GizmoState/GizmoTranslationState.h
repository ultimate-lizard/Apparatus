#pragma once

#include "GizmoState.h"

class GizmoTranslationState : public GizmoState
{
public:
	GizmoTranslationState(SnappingData& snappingData);

	virtual void handleGizmoClick(const glm::vec3& clickWorldPosition) override;
	virtual void handleGizmoRelease() override {}
	virtual void handleCursorMovement(const glm::ivec2& cursorPosition, Camera* camera, TransformComponent* selectedEntityTransformComponent) override;

protected:
	virtual void updateVisibility() override;

	glm::vec3 clickOffset;
	glm::vec3 positionDuringClick;
};
