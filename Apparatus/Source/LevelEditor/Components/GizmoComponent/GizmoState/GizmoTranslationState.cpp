#include "GizmoTranslationState.h"

#include <Apparatus/Server/Entity.h>
#include <Apparatus/Rendering/Camera.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Util/CollisionDetection.h>

#include "../GizmoNames.h"

GizmoTranslationState::GizmoTranslationState(SnappingData& snappingData) :
	GizmoState(snappingData),
	clickOffset(0.0f),
	positionDuringClick(0.0f)
{

}

void GizmoTranslationState::updateVisibility()
{
	if (gizmo)
	{
		for (ModelComponent* elementModelComponent : gizmo->getAllComponentsOfClass<ModelComponent>())
		{
			if (!elementModelComponent)
			{
				continue;
			}

			elementModelComponent->setVisibility(false);

			const std::string modelName = elementModelComponent->getComponentName();
			elementModelComponent->setVisibility((modelName.find(GizmoNames::Gizmo) != std::string::npos) && gizmoVisible);
		}
	}
}

void GizmoTranslationState::handleGizmoClick(const glm::vec3& clickWorldPosition)
{
	if (selectedGizmoModelComponent)
	{
		clickOffset = (clickWorldPosition - selectedGizmoModelComponent->getWorldPosition()) / selectedGizmoModelComponent->getWorldScale();
		positionDuringClick = selectedGizmoModelComponent->getWorldPosition();
	}
}

void GizmoTranslationState::handleCursorMovement(const glm::ivec2& cursorPosition, Camera* camera, TransformComponent* selectedEntityTransformComponent)
{
	if (!selectedGizmoModelComponent || !camera || !selectedEntityTransformComponent)
	{
		return;
	}

	// Convert all required vectors into gizmo's local space for the easier creation of the gizmo axis normals
	glm::vec3 rayOriginLocal = camera->getPosition();
	glm::vec3 rayDirectionLocal = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));
	glm::vec3 gizmoOriginLocal = selectedEntityTransformComponent->getWorldPosition();
	glm::vec3 gizmoSelectPositionOriginLocal = positionDuringClick;

	// No need to change offsets to local space. Only rotate
	glm::vec3 gizmoClickOffsetLocal = clickOffset * selectedGizmoModelComponent->getWorldScale();

	// Convert to gizmo's local space (for easier way of getting the proper axis plane normal)
	if (SceneNode* parent = selectedEntityTransformComponent->getParent())
	{
		rayOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayOriginLocal, 1.0f);
		rayDirectionLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayDirectionLocal, 0.0f);
		gizmoOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoOriginLocal, 1.0f);
		gizmoSelectPositionOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoSelectPositionOriginLocal, 1.0f);

		// No need to convert to local coordiante system since this is just an offset. But need to rotate it
		gizmoClickOffsetLocal = glm::inverse(parent->getWorldOrientation()) * gizmoClickOffsetLocal / parent->getWorldScale();
	}

	glm::vec3 axisPlaneNormal = glm::normalize(rayOriginLocal - gizmoSelectPositionOriginLocal);

	const std::string selectedGizmoName = selectedGizmoModelComponent->getComponentName();
	if (selectedGizmoName == GizmoNames::GizmoX)
	{
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXY)
	{
		axisPlaneNormal.y = 0.0f;
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXZ)
	{
		axisPlaneNormal.z = 0.0f;
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoY)
	{
		axisPlaneNormal.y = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZY)
	{
		axisPlaneNormal.y = 0.0f;
		axisPlaneNormal.z = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZ)
	{
		axisPlaneNormal.z = 0.0f;
	}

	glm::vec3 newPosition = rayVsPlane(gizmoSelectPositionOriginLocal, axisPlaneNormal, rayOriginLocal, rayDirectionLocal);
	if (glm::any(glm::isnan(newPosition)))
	{
		return;
	}

	newPosition -= gizmoClickOffsetLocal;

	if (selectedGizmoName == GizmoNames::GizmoX)
	{
		newPosition.y = gizmoOriginLocal.y;
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXY)
	{
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXZ)
	{
		newPosition.y = gizmoOriginLocal.y;
	}
	else if (selectedGizmoName == GizmoNames::GizmoY)
	{
		newPosition.x = gizmoOriginLocal.x;
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZY)
	{
		newPosition.x = gizmoOriginLocal.x;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZ)
	{
		newPosition.x = gizmoOriginLocal.x;
		newPosition.y = gizmoOriginLocal.y;
	}

	// Snapping
	if (snappingData.snapToGrid)
	{
		newPosition = glm::round(newPosition / snappingData.translationStep) * snappingData.translationStep;
	}

	selectedEntityTransformComponent->setPosition(newPosition);
}
