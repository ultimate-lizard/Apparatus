#include "GizmoScaleState.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Server/Entity.h>
#include <Apparatus/Rendering/Camera.h>
#include <Apparatus/Rendering/Vector.h>
#include <Apparatus/Util/CollisionDetection.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>

#include "../GizmoNames.h"

GizmoScaleState::GizmoScaleState(SnappingData& snappingData) :
	GizmoState(snappingData),
	clickPosition(0.0f),
	positionDuringClick(0.0f),
	lastCursorDevicePosition(0.0f),
	lastCursorPosition(0)
{
}

void GizmoScaleState::updateVisibility()
{
	if (gizmo)
	{
		for (ModelComponent* elementModelComponent : gizmo->getAllComponentsOfClass<ModelComponent>())
		{
			if (!elementModelComponent)
			{
				continue;
			}

			const std::string modelName = elementModelComponent->getComponentName();
			elementModelComponent->setVisibility((modelName.find(GizmoNames::Scale) != std::string::npos) && gizmoVisible);
		}
	}
}

void GizmoScaleState::handleGizmoClick(const glm::vec3& clickWorldPosition)
{
	clickPosition = clickWorldPosition;
	lastCursorDevicePosition = getCursorToDevice();

	if (selectedGizmoModelComponent)
	{
		positionDuringClick = selectedGizmoModelComponent->getWorldPosition();
	}

	lastCursorPosition = Apparatus::getWindow().getMouseCursorPosition();
	Apparatus::getWindow().setCursorVisibleEnabled(false);
}

void GizmoScaleState::handleGizmoRelease()
{
	Apparatus::getWindow().setCursorVisibleEnabled(true);
}

void GizmoScaleState::handleCursorMovement(const glm::ivec2& cursorPosition, Camera* camera, TransformComponent* selectedEntityTransformComponent)
{
	if (!selectedGizmoModelComponent || !camera || !selectedEntityTransformComponent)
	{
		return;
	}

	glm::vec3 newScale(0.0f);
	if (selectedGizmoModelComponent->getComponentName() == GizmoNames::ScaleAll)
	{
		glm::vec2 cursorDragPosition = getCursorToDevice();
		glm::vec2 cursorDragOffset = (cursorDragPosition - lastCursorDevicePosition) * 2.0f;

		newScale = snappingData.unsnappedScale * (cursorDragOffset.x + cursorDragOffset.y + 1.0f);
	}
	else
	{
		// Convert all required vectors into gizmo's local space for the easier creation of the gizmo axis normals
		glm::vec3 rayOrigin = camera->getPosition();
		glm::vec3 rayDirection = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));

		glm::vec3 axisPlaneNormal = glm::normalize(rayOrigin - clickPosition);

		const std::string selectedGizmoName = selectedGizmoModelComponent->getComponentName();

		glm::vec3 dragPositionLocal = rayVsPlane(clickPosition, axisPlaneNormal, rayOrigin, rayDirection);
		if (glm::any(glm::isnan(dragPositionLocal)))
		{
			return;
		}

		// Determine whether the scaling must be negated or not
		glm::vec3 clickOffset = clickPosition - positionDuringClick;
		glm::vec3 scaleOffset = dragPositionLocal - clickPosition;

		const int longestAxis = Vector::findLongestAxis(scaleOffset);
		if (clickOffset[longestAxis] < 0.0f)
		{
			scaleOffset = -scaleOffset;
		}

		scaleOffset.x = scaleOffset[longestAxis];
		scaleOffset.y = scaleOffset[longestAxis];
		scaleOffset.z = scaleOffset[longestAxis];

		newScale = snappingData.unsnappedScale + scaleOffset;

		const glm::vec3 gizmoCurrentScale = snappingData.unsnappedScale;
		if (selectedGizmoName == GizmoNames::ScaleUp || selectedGizmoName == GizmoNames::ScaleDown)
		{
			newScale.x = gizmoCurrentScale.x;
			newScale.z = gizmoCurrentScale.z;
		}
		else if (selectedGizmoName == GizmoNames::ScaleRight || selectedGizmoName == GizmoNames::ScaleLeft)
		{
			newScale.y = gizmoCurrentScale.y;
			newScale.z = gizmoCurrentScale.z;
		}
		else if (selectedGizmoName == GizmoNames::ScaleFront || selectedGizmoName == GizmoNames::ScaleBack)
		{
			newScale.x = gizmoCurrentScale.x;
			newScale.y = gizmoCurrentScale.y;
		}
	}

	snappingData.unsnappedScale = newScale;

	if (snappingData.snapToGrid)
	{
		glm::vec3 snappedScale = glm::round(snappingData.unsnappedScale / snappingData.scaleStep) * snappingData.scaleStep;
		selectedEntityTransformComponent->setScale(snappedScale);
	}
	else
	{
		selectedEntityTransformComponent->setScale(newScale);
	}

	Apparatus::getWindow().setCursorPosition(lastCursorPosition);
}
