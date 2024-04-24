#pragma once

#include <glm/glm.hpp>

#include "../SnappingData.h"

class TransformComponent;
class ModelComponent;
class Camera;
class Entity;

class GizmoState
{
public:
	GizmoState(SnappingData& snappingData);
	virtual ~GizmoState() = default;

	virtual void handleGizmoClick(const glm::vec3& clickWorldPosition) = 0;
	virtual void handleGizmoRelease() = 0;
	virtual void handleCursorMovement(const glm::ivec2& cursorPosition, Camera* camera, TransformComponent* selectedEntityTransformComponent) = 0;

	void setSelectedGizmoModelComponent(ModelComponent* selectedGizmoModelComponent);
	ModelComponent* getSelectedGizmoModelComponent() const;

	void setGizmoVisibility(bool visible);

protected:
	virtual void updateVisibility() = 0;

	ModelComponent* selectedGizmoModelComponent;
	Entity* gizmo;
	SnappingData& snappingData;

	bool gizmoVisible;

	friend class GizmoComponent;
};
