#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>

#include <Apparatus/Components/Component.h>
#include <Apparatus/Rendering/Rotator.h>

#include "GizmoComponent/SnappingData.h"
#include "GizmoComponent/GizmoState/GizmoState.h"

class ModelComponent;
class Window;
class Camera;
class TransformComponent;

enum class InteractionMode
{
	Translation,
	Rotation,
	Scale
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

	void setVisibility(bool enabled);
	void attach(Entity* parent);

	void press(const glm::vec3& clickWorldPosition);
	void release();

	bool isPressed() const;

	void handleCursorMovement(float inputX, float inputY);

	void setSnapToGridEnabled(bool enabled);

	void setSelectedEntity(Entity* selectedEntity);

	template <class StateClass>
	void changeState();

protected:
	std::unique_ptr<GizmoState> gizmoState;

	InteractionMode interactionMode;

	ModelComponent* selectedGizmoModel;

	bool pressed;
	bool gizmoVisible;
	
	Entity* selectedEntity;

	// This is a rotation value used during rotation manipulation. A rounded version is used to set rotation when grid enabled
	SnappingData snappingData;
};

template<class StateClass>
inline void GizmoComponent::changeState()
{
	gizmoState = std::make_unique<StateClass>(snappingData);
	gizmoState->gizmo = getOwner();
	gizmoState->setGizmoVisibility(gizmoVisible);
}
