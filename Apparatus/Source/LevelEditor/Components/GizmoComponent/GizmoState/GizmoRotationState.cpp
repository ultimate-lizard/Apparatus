#include "GizmoRotationState.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Util/CollisionDetection.h>
#include <Apparatus/Server/Entity.h>
#include <Apparatus/Rendering/Vector.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>

#include "../GizmoNames.h"

GizmoRotationState::GizmoRotationState(SnappingData& snappingData) :
	GizmoState(snappingData),
	clickPosition(0.0f),
	clickAngle(0.0f),
	lastCursorPosition(0)
{
}

void GizmoRotationState::updateVisibility()
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
			elementModelComponent->setVisibility((modelName == GizmoNames::Pitch || modelName == GizmoNames::Yaw || modelName == GizmoNames::Roll) && gizmoVisible);
		}
	}
}

void GizmoRotationState::handleGizmoClick(const glm::vec3& clickWorldPosition)
{
	if (!selectedGizmoModelComponent)
	{
		return;
	}

	Apparatus& app = Apparatus::get();
	if (LocalClient* localClient = app.getPrimaryLocalClient())
	{
		if (Camera* camera = localClient->getActiveCamera())
		{
			glm::vec3 rayDirectionLocal = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));
			glm::vec3 rayOriginLocal = camera->getWorldPosition();
			glm::vec3 camRightLocal = camera->getRight();

			// Convert to gimbal's local space
			if (SceneNode* parent = selectedGizmoModelComponent->getParent())
			{
				rayDirectionLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayDirectionLocal, 0.0f);
				rayOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayOriginLocal, 1.0f);
				camRightLocal = glm::inverse(parent->getTransform()) * glm::vec4(camRightLocal, 1.0f);
			}

			const glm::vec3 gizmoOriginLocal = selectedGizmoModelComponent->getPosition();
			const glm::vec3 gimbalUpLocal = getLocalUpForGimbal(selectedGizmoModelComponent->getComponentName());

			clickPosition = rayVsPlane(gizmoOriginLocal, gimbalUpLocal, rayOriginLocal, rayDirectionLocal);

			glm::vec3 clickToGimbalProjection = clickPosition - gizmoOriginLocal;
			const glm::vec2 clickToGimbalProjection2d = projectOnPlane(clickToGimbalProjection, gimbalUpLocal);
			clickAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(clickToGimbalProjection2d.x, clickToGimbalProjection2d.y)));
		}
	}

	lastCursorPosition = Apparatus::getWindow().getMouseCursorPosition();
	Apparatus::getWindow().setCursorVisibleEnabled(false);
}

void GizmoRotationState::handleGizmoRelease()
{
	Apparatus::getWindow().setCursorVisibleEnabled(true);

	// Rotate the scale models
	if (selectedGizmoModelComponent && gizmo)
	{
		for (ModelComponent* modelComponent : gizmo->getAllComponentsOfClass<ModelComponent>())
		{
			if (!modelComponent)
			{
				continue;
			}

			if (modelComponent->getComponentName().find(GizmoNames::Scale) != std::string::npos)
			{
				modelComponent->setRotation(selectedGizmoModelComponent->getRotator());
			}
		}
	}
}

void GizmoRotationState::handleCursorMovement(const glm::ivec2& cursorPosition, Camera* camera, TransformComponent* selectedEntityTransformComponent)
{
	if (!selectedGizmoModelComponent || !camera || !selectedEntityTransformComponent)
	{
		return;
	}

	glm::vec3 rayDirection = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));
	glm::vec3 rayOrigin = camera->getWorldPosition();
	glm::vec3 gizmoOrigin = selectedGizmoModelComponent->getPosition();
	glm::vec3 gizmoUp = getLocalUpForGimbal(selectedGizmoModelComponent->getComponentName());

	if (SceneNode* parent = selectedGizmoModelComponent->getParent())
	{
		rayDirection = glm::inverse(parent->getTransform()) * glm::vec4(rayDirection, 0.0f);
		rayOrigin = glm::inverse(parent->getTransform()) * glm::vec4(rayOrigin, 1.0f);
	}

	glm::vec3 dragPosition = rayVsPlane(gizmoOrigin, gizmoUp, rayOrigin, rayDirection);
	if (glm::any(glm::isnan(dragPosition)))
	{
		return;
	}

	glm::vec3 dragToGimbalProjection = dragPosition - gizmoOrigin;
	const glm::vec2 dragToGimbalProjection2d = projectOnPlane(dragToGimbalProjection, gizmoUp);

	float dragAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(dragToGimbalProjection2d.x, dragToGimbalProjection2d.y)));

	float deltaAngle = clickAngle - dragAngle;

	// We need to handle the case when comparing two numbers that stand to the left and right from 0 degrees
	if (glm::abs(deltaAngle) > 180.0f)
	{
		if (deltaAngle < 0.0f)
		{
			deltaAngle = 360.0f - glm::abs(deltaAngle);
		}
		else
		{
			deltaAngle = 360.0f - glm::abs(deltaAngle);
			deltaAngle = -deltaAngle;
		}
	}

	if (selectedGizmoModelComponent->getComponentName() == GizmoNames::Yaw)
	{
		deltaAngle = -deltaAngle;
	}

	// Sensitivity
	deltaAngle *= 0.1f;

	// Snapping
	const Euler gimbalAngle = getEulerAngleOfGimbal(selectedGizmoModelComponent->getComponentName());

	snappingData.unsnappedRotator.rotate(deltaAngle, gimbalAngle);

	if (snappingData.snapToGrid)
	{
		float angleSnapped = snappingData.unsnappedRotator.get(gimbalAngle);
		angleSnapped = glm::round(angleSnapped / snappingData.rotationStep) * snappingData.rotationStep;

		selectedEntityTransformComponent->setRotation(angleSnapped, gimbalAngle);
		selectedGizmoModelComponent->setRotation(angleSnapped, gimbalAngle);
	}
	else
	{
		selectedEntityTransformComponent->rotate(deltaAngle, gimbalAngle);
		selectedGizmoModelComponent->rotate(deltaAngle, gimbalAngle);
	}

	Apparatus::getWindow().setCursorPosition(lastCursorPosition);
}

glm::vec3 GizmoRotationState::getLocalUpForGimbal(const std::string& gimbalName) const
{
	if (selectedGizmoModelComponent)
	{
		if (gimbalName == GizmoNames::Pitch)
		{
			return selectedGizmoModelComponent->getLocalRight();
		}
		else if (gimbalName == GizmoNames::Yaw)
		{
			return selectedGizmoModelComponent->getLocalUp();
		}
		else if (gimbalName == GizmoNames::Roll)
		{
			return selectedGizmoModelComponent->getLocalForward();
		}
	}

	return glm::vec3(std::numeric_limits<float>::quiet_NaN());
}

glm::vec2 GizmoRotationState::projectOnPlane(const glm::vec3& position, const glm::vec3& normal) const
{
	glm::vec2 result;

	glm::vec3 test = position;

	int upAxis = Vector::findLongestAxis(glm::abs(normal));

	test[upAxis] = std::numeric_limits<float>::quiet_NaN();

	result.x = glm::isnan(test.x) ? test.y : test.x;
	result.y = (result.x == test.y) ? test.z : (glm::isnan(test.y) ? test.z : test.y);

	return result;
}

Euler GizmoRotationState::getEulerAngleOfGimbal(const std::string& gimbalName) const
{
	if (gimbalName == GizmoNames::Pitch)
	{
		return Euler::Pitch;
	}
	else if (gimbalName == GizmoNames::Yaw)
	{
		return Euler::Yaw;
	}
	else if (gimbalName == GizmoNames::Roll)
	{
		return Euler::Roll;
	}

	return Euler::Pitch;
}
