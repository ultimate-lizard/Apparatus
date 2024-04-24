#include "GizmoState.h"

GizmoState::GizmoState(SnappingData& snappingData) :
	selectedGizmoModelComponent(nullptr),
	gizmo(nullptr),
	snappingData(snappingData),
	gizmoVisible(false)
{
}

void GizmoState::setSelectedGizmoModelComponent(ModelComponent* selectedGizmoModelComponent)
{
	this->selectedGizmoModelComponent = selectedGizmoModelComponent;
}

ModelComponent* GizmoState::getSelectedGizmoModelComponent() const
{
	return selectedGizmoModelComponent;
}

void GizmoState::setGizmoVisibility(bool visible)
{
	gizmoVisible = visible;
	updateVisibility();
}
