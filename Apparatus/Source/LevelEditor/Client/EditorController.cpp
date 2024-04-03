#include "EditorController.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Apparatus/Apparatus.h>
#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Components/CameraComponent.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/MovementComponent.h>
#include <Apparatus/Server/Entity.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Util/DebugPrimitive.h>
#include <Apparatus/Util/CollisionDetection.h>

#include "EditorLocalClient.h"
#include "../Components/SelectableComponent.h"

static float clickAngle = 0.0f;
static float rightAngle = 0.0f;
// static glm::vec2 initialDevice;
// bool inverseDirection;
// bool resetAtan = false;
// static float rotOffset = 0.0f;
// static glm::vec3 initPos;
glm::vec3 clickPosition;

EditorController::EditorController(EditorLocalClient* editorLocalClient) :
	HumanControllerBase(editorLocalClient),
	editorLocalClient(editorLocalClient),
	gizmoPressed(false),
	selectedGizmoModel(nullptr)
{
	assignDefaultObjectName();
}

void EditorController::assignDefaultObjectName()
{
	setObjectName("EditorController");
}

void EditorController::onActivate()
{
	HumanControllerBase::onActivate();
	
	if (localClient)
	{
		if (Apparatus* apparatus = localClient->getApparatus())
		{
			apparatus->setCursorVisibleEnabled(true);
		}
	}
}

void EditorController::onDeactivate()
{
	if (gizmoPressed)
	{
		releasePrimaryMouseButton();
	}
}

void EditorController::setupInput()
{
	if (!inputHandler)
	{
		return;
	}

	inputHandler->bindKeyAction("Fire", KeyEventType::Press, std::bind(&EditorController::pressSelect, this));
	inputHandler->bindKeyAction("Fire", KeyEventType::Release, std::bind(&EditorController::releasePrimaryMouseButton, this));

	inputHandler->bindKeyAction("AltFire", KeyEventType::Press, std::bind(&EditorController::releaseSelect, this));
	inputHandler->bindKeyAction("OpenMainMenu", KeyEventType::Press, std::bind(&EditorController::releaseSelect, this));

	inputHandler->bindKeyAction("EnableTranslationMode", KeyEventType::Press, std::bind(&EditorController::enableTranslationMode, this));
	inputHandler->bindKeyAction("EnableRotationMode", KeyEventType::Press, std::bind(&EditorController::enableRotationMode, this));

	inputHandler->bindAxisAction("LookY", std::bind(&EditorController::cursorMoveY, this, std::placeholders::_1));
	inputHandler->bindAxisAction("LookX", std::bind(&EditorController::cursorMoveX, this, std::placeholders::_1));

	inputHandler->bindKeyAction("ToggleDebugPrimitives", KeyEventType::Press, std::bind(&EditorController::toggleDebugPrimitives, this));
}

void EditorController::pressSelect()
{
	if (!editorLocalClient)
	{
		return;
	}

	Apparatus* apparatus = editorLocalClient->getApparatus();
	if (!apparatus)
	{
		return;
	}

	LocalServer* localServer = apparatus->getServer<LocalServer>();
	if (!localServer)
	{
		return;
	}

	Camera* camera = editorLocalClient->getActiveCamera();
	if (!camera)
	{
		return;
	}

	glm::vec3 direction = localServer->getCursorToWorldRay(camera->getView(), camera->getProjection());
	glm::vec3 origin = camera->getWorldPosition();

	std::vector<RayTraceResult> traceResults = localServer->traceRay(origin, direction, DetectionType::Visibility);

	bool gizmoTraced = false;
	for (const RayTraceResult& result : traceResults)
	{
		if (result.entity->getObjectName() == "Gizmo")
		{
			gizmoTraced = true;

			selectedGizmoModel = result.modelComponent;
			if (selectedGizmoModel)
			{
				gizmoClickOffset = result.near - selectedGizmoModel->getWorldPosition();
				gizmoSelectPositionOrigin = selectedGizmoModel->getWorldPosition();
			}
			break;
		}
	}

	if (gizmoTraced)
	{
		gizmoPressed = true;

		if (selectedGizmoModel)
		{
			if (editorLocalClient->getInteractionMode() == InteractionMode::Rotation)
			{
				lastMousePosition = apparatus->getMouseCursorPosition();
				apparatus->setCursorVisibleEnabled(false);

				const glm::vec3 rayDirection = glm::normalize(localClient->getLocalServer()->getCursorToWorldRay(camera->getView(), camera->getProjection()));
				const glm::vec3 rayOrigin = camera->getWorldPosition();
				clickPosition = rayVsPlane(selectedGizmoModel->getWorldPosition(), getSelectedGimbalUp(), rayOrigin, rayDirection);

				const glm::vec3 clickToGimbalProjection = clickPosition - selectedGizmoModel->getWorldPosition();
				//drawDebugLine(selectedGizmoModel->getDerivedPosition(), selectedGizmoModel->getDerivedPosition() + clickToGimbalProjection * 5.0f, glm::vec4(0.0f), 2.0f, true);
				const glm::vec2 clickToGimbalProjection2d = projectOnGimbalPlane(clickToGimbalProjection);

				clickAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(clickToGimbalProjection2d.x, clickToGimbalProjection2d.y)));

				const glm::vec3 camRight = camera->getRight();
				const glm::vec3 rightToGimbalProjection = camRight - (glm::dot(camRight, getSelectedGimbalUp()) * getSelectedGimbalUp());
				//drawDebugLine(selectedGizmoModel->getDerivedPosition(), selectedGizmoModel->getDerivedPosition() + rightToGimbalProjection * 5.0f, glm::vec4(1.0f), 2.0f, true);
				const glm::vec2 rightToGimbalProjection2d = projectOnGimbalPlane(rightToGimbalProjection);
				
				rightAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(rightToGimbalProjection2d.x, rightToGimbalProjection2d.y)));

				LOG("Right: " + std::to_string(rightAngle), LogLevel::Info);
				LOG("Click: " + std::to_string(clickAngle), LogLevel::Info);
			}
		}
	}
	else
	{
		// Select the closest entity to the ray origin. Index 0 is the closest
		editorLocalClient->selectEntity(!traceResults.empty() ? traceResults[0].entity : nullptr);
	}
}

void EditorController::releaseSelect()
{
	if (editorLocalClient)
	{
		editorLocalClient->selectEntity(nullptr);
	}
}

void EditorController::toggleDebugPrimitives()
{
	if (editorLocalClient)
	{
		editorLocalClient->showDebugPrimitives = !editorLocalClient->showDebugPrimitives;
	}
}

void EditorController::enableTranslationMode()
{
	if (editorLocalClient)
	{
		editorLocalClient->setInteractionMode(InteractionMode::Translation);
	}
}

void EditorController::enableRotationMode()
{
	if (editorLocalClient)
	{
		editorLocalClient->setInteractionMode(InteractionMode::Rotation);
	}
}

void EditorController::releasePrimaryMouseButton()
{
	if (editorLocalClient)
	{
		if (editorLocalClient->getInteractionMode() == InteractionMode::Rotation)
		{
			if (gizmoPressed)
			{
				if (Apparatus* apparatus = editorLocalClient->getApparatus())
				{
					apparatus->setCursorVisibleEnabled(true);
				}
			}

			if (Entity* selectedEntity = editorLocalClient->getSelectedEntity())
			{
				if (auto selectableComponent = selectedEntity->findComponent<SelectableComponent>())
				{
					selectableComponent->regenerateVisualBoundingBox();
					selectableComponent->setBoxVisible(true);
				}
			}
		}
	}

	gizmoPressed = false;
}

void EditorController::cursorMoveY(float value)
{
	cursorMove(0.0f, value);
}

void EditorController::cursorMoveX(float value)
{
	cursorMove(value, 0.0f);
}

void EditorController::cursorMove(float mouseInputX, float mouseInputY)
{
	if (!gizmoPressed || !editorLocalClient)
	{
		return;
	}

	const InteractionMode mode = editorLocalClient->getInteractionMode();
	if (mode == InteractionMode::Translation)
	{
		handleGizmoTranslation();
	}
	else if (mode == InteractionMode::Rotation)
	{
		handleGizmoRotation(mouseInputX, mouseInputY);
	}
}

void EditorController::handleGizmoTranslation()
{
	if (!selectedGizmoModel || !editorLocalClient)
	{
		return;
	}

	Camera* camera = editorLocalClient->getActiveCamera();
	if (!camera)
	{
		return;
	}

	Entity* selectedEntity = editorLocalClient->getSelectedEntity();
	if (!selectedEntity)
	{
		return;
	}

	TransformComponent* selectedEntityTransform = selectedEntity->findComponent<TransformComponent>();
	if (!selectedEntityTransform)
	{
		return;
	}

	// Convert all required vectors into gizmo's local space for the easier creation of the gizmo axis normals
	glm::vec3 rayOriginLocal = camera->getWorldPosition();
	glm::vec3 rayDirectionLocal = glm::normalize(localClient->getLocalServer()->getCursorToWorldRay(camera->getView(), camera->getProjection()));
	glm::vec3 gizmoOriginLocal = selectedEntityTransform->getWorldPosition();

	glm::vec3 gizmoSelectPositionOriginLocal = gizmoSelectPositionOrigin;
	// No need to change offsets to local space. Only rotate
	glm::vec3 gizmoClickOffsetLocal = gizmoClickOffset;

	if (SceneNode* parent = selectedEntityTransform->getParent())
	{
		rayOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayOriginLocal, 1.0f);
		rayDirectionLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayDirectionLocal, 0.0f);
		gizmoOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoOriginLocal, 1.0f);

		gizmoSelectPositionOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoSelectPositionOriginLocal, 1.0f);

		// No need to convert to local coordiante system since this is just an offset. But need to rotate it
		gizmoClickOffsetLocal = glm::inverse(parent->getDerivedOrientation()) * gizmoClickOffsetLocal;
	}

	glm::vec3 axisPlaneNormal = glm::normalize(rayOriginLocal - gizmoSelectPositionOriginLocal);

	const std::string selectedGizmoName = selectedGizmoModel->getObjectName();
	if (selectedGizmoName == "GizmoX")
	{
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == "GizmoXY")
	{
		axisPlaneNormal.y = 0.0f;
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == "GizmoXZ")
	{
		axisPlaneNormal.z = 0.0f;
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == "GizmoY")
	{
		axisPlaneNormal.y = 0.0f;
	}
	else if (selectedGizmoName == "GizmoYZ")
	{
		axisPlaneNormal.y = 0.0f;
		axisPlaneNormal.z = 0.0f;
	}
	else if (selectedGizmoName == "GizmoZ")
	{
		axisPlaneNormal.z = 0.0f;
	}

	glm::vec3 newPosition = rayVsPlane(gizmoSelectPositionOriginLocal, axisPlaneNormal, rayOriginLocal, rayDirectionLocal);
	if (glm::all(glm::isnan(newPosition)))
	{
		return;
	}

	newPosition -= gizmoClickOffsetLocal;

	if (selectedGizmoName == "GizmoX")
	{
		newPosition.y = gizmoOriginLocal.y;
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == "GizmoXY")
	{
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == "GizmoXZ")
	{
		newPosition.y = gizmoOriginLocal.y;
	}
	else if (selectedGizmoName == "GizmoY")
	{
		newPosition.x = gizmoOriginLocal.x;
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == "GizmoYZ")
	{
		newPosition.x = gizmoOriginLocal.x;
	}
	else if (selectedGizmoName == "GizmoZ")
	{
		newPosition.x = gizmoOriginLocal.x;
		newPosition.y = gizmoOriginLocal.y;
	}

	// glm::vec3 a = newPosition - gizmoOriginLocal;

	selectedEntityTransform->setPosition(newPosition);
}

void EditorController::handleGizmoRotation(float mouseInputX, float mouseInputY)
{
	if (!selectedGizmoModel)
	{
		return;
	}

	Camera* camera = editorLocalClient->getActiveCamera();
	if (!camera)
	{
		return;
	}

	if (Entity* selection = editorLocalClient->getSelectedEntity())
	{
		if (auto selectableComponent = selection->findComponent<SelectableComponent>())
		{
			selectableComponent->setBoxVisible(false);
		}

		if (auto selectionTransform = selection->findComponent<TransformComponent>())
		{
			float right = rightAngle;
			float left = Rotator::normalizeAngle(rightAngle + 180.0f);

			Euler gimbalAngle = getSelectedGimbalEulerAngle();

			const glm::vec3 rayDirection = glm::normalize(localClient->getLocalServer()->getCursorToWorldRay(camera->getView(), camera->getProjection()));
			const glm::vec3 rayOrigin = camera->getWorldPosition();
			glm::vec3 dragPos = rayVsPlane(selectedGizmoModel->getWorldPosition(), getSelectedGimbalUp(), rayOrigin, rayDirection);

			const glm::vec3 dragToGimbalProjection = dragPos - selectedGizmoModel->getWorldPosition();
			drawDebugLine(selectedGizmoModel->getWorldPosition(), selectedGizmoModel->getWorldPosition() + getSelectedGimbalUp() * 15.0f, glm::vec4(0.0f), 2.0f, false);
			const glm::vec2 dragToGimbalProjection2d = projectOnGimbalPlane(dragToGimbalProjection);

			float dragAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(dragToGimbalProjection2d.x, dragToGimbalProjection2d.y)));

			float deltaAngle = clickAngle - dragAngle;

			selectionTransform->rotate(deltaAngle, gimbalAngle);
			selectedGizmoModel->rotate(deltaAngle, gimbalAngle);

			if (Apparatus* app = editorLocalClient->getApparatus())
			{
				app->setCursorPosition(lastMousePosition);
			}
		}
	}
}

float EditorController::getPositionAngle(const glm::vec3& center, const glm::vec2& position)
{
	float yaw = glm::degrees(glm::atan(position.x, position.y));
	return Rotator::normalizeAngle(yaw);
}

Euler EditorController::getSelectedGimbalEulerAngle() const
{
	const std::string selectedGizmoName = selectedGizmoModel->getObjectName();

	if (selectedGizmoName == "Pitch")
	{
		return Euler::Pitch;
	}
	else if (selectedGizmoName == "Yaw")
	{
		return Euler::Yaw;
	}
	else if (selectedGizmoName == "Roll")
	{
		return Euler::Roll;
	}

	return Euler::Pitch;
}

glm::vec3 EditorController::getSelectedGimbalUp() const
{
	if (selectedGizmoModel)
	{
		const std::string selectedGizmoName = selectedGizmoModel->getObjectName();

		if (selectedGizmoName == "Pitch")
		{
			return selectedGizmoModel->getRight();
		}
		else if (selectedGizmoName == "Yaw")
		{
			return selectedGizmoModel->getUp();
		}
		else if (selectedGizmoName == "Roll")
		{
			return selectedGizmoModel->getForward();
		}
	}

	return glm::vec3(0.0f);
}

glm::vec3 EditorController::getSelectedGimbalLocalUp() const
{
	if (selectedGizmoModel)
	{
		const std::string selectedGizmoName = selectedGizmoModel->getObjectName();

		if (selectedGizmoName == "Pitch")
		{
			return selectedGizmoModel->getLocalRight();
		}
		else if (selectedGizmoName == "Yaw")
		{
			return selectedGizmoModel->getLocalUp();
		}
		else if (selectedGizmoName == "Roll")
		{
			return selectedGizmoModel->getLocalForward();
		}
	}

	return glm::vec3(0.0f);
}

glm::vec2 EditorController::directionRelativeToGimbal(const glm::vec3& direction) const
{
	glm::vec3 norm = selectedGizmoModel->getWorldPosition() + direction;

	if (selectedGizmoModel)
	{
		const std::string selectedGizmoName = selectedGizmoModel->getObjectName();

		if (selectedGizmoName == "Pitch")
		{
			return { norm.z, norm.y };
		}
		else if (selectedGizmoName == "Yaw")
		{
			return { norm.x, norm.z };
		}
		else if (selectedGizmoName == "Roll")
		{
			return { norm.y, norm.x };
		}
	}

	return norm;
}

glm::vec2 EditorController::projectOnGimbalPlane(const glm::vec3& position) const
{
	glm::vec2 result;

	glm::vec3 test = position;
	
	glm::vec3 planeNormal = glm::abs(getSelectedGimbalUp());
	int upAxis = 0;

	float max = std::max({ planeNormal.x, planeNormal.y, planeNormal.z });
	for (int i = 0; i < 3; ++i)
	{
		if (planeNormal[i] == max)
		{
			upAxis = i;
		}
	}

	test[upAxis] = std::numeric_limits<float>::quiet_NaN();

	result.x = glm::isnan(test.x) ? test.y : test.x;
	result.y = (result.x == test.y) ? test.z : (glm::isnan(test.y) ? test.z : test.y);

	return result;
}
