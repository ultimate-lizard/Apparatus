#include "EditorController.h"

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

static Rotator gizmoSelectRotationOrigin;
static float clickRotator;

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
	glm::vec3 origin = camera->getDerivedPosition();

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
				clickOffset = result.near - selectedGizmoModel->getDerivedPosition();
			}
			// TODO: IsRotationGizmo?

			break;
		}
	}

	if (gizmoTraced)
	{
		gizmoPressed = true;

		if (selectedGizmoModel)
		{
			gizmoSelectPositionOrigin = selectedGizmoModel->getDerivedPosition();
			gizmoSelectRotationOrigin = selectedGizmoModel->getRotator();

			const glm::vec3 rayOrigin = camera->getDerivedPosition();
			const glm::vec3 rayDirection = glm::normalize(localClient->getLocalServer()->getCursorToWorldRay(camera->getView(), camera->getProjection()));
			glm::vec3 newPosition = rayVsPlane(selectedGizmoModel->getDerivedPosition(), getSelectedGimbalUp(), rayOrigin, rayDirection);

			float positionAngle = getPositionAngle(selectedGizmoModel->getDerivedPosition(), positionRelativeToGimbal(newPosition));

			clickRotator = Rotator::normalizeAngle(positionAngle);
			
			// TODO: MOUSE PICKING IS BAD? MAYBE TRY DIRECTIONAL VECTOR INSTEAD
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
	gizmoPressed = false;

	if (editorLocalClient)
	{
		if (editorLocalClient->getInteractionMode() == InteractionMode::Rotation)
		{
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

	const std::string selectedGizmoName = selectedGizmoModel->getObjectName();

	glm::vec3 planeNormal(0.0f);

	const glm::vec3 rayOrigin = camera->getDerivedPosition();
	const glm::vec3 rayDirection = glm::normalize(localClient->getLocalServer()->getCursorToWorldRay(camera->getView(), camera->getProjection()));
	const glm::vec3 gizmoOrigin = selectedGizmoModel->getDerivedPosition();

	planeNormal = glm::normalize(rayOrigin - gizmoOrigin);

	const glm::vec3 origNormal = planeNormal;

	if (editorLocalClient->showDebugPrimitives)
	{
		drawDebugLine(gizmoOrigin, gizmoOrigin + planeNormal * 10.0f, { 0.0f, 0.0f, 1.0f, 1.0f }, 2.0f, false, 5.0f);
	}

	if (selectedGizmoName == "GizmoX")
	{
		planeNormal.x = 0.0f;
	}
	else if (selectedGizmoName == "GizmoXY")
	{
		planeNormal.y = 0.0f;
		planeNormal.x = 0.0f;
	}
	else if (selectedGizmoName == "GizmoXZ")
	{
		planeNormal.z = 0.0f;
		planeNormal.x = 0.0f;
	}
	else if (selectedGizmoName == "GizmoY")
	{
		planeNormal.y = 0.0f;
	}
	else if (selectedGizmoName == "GizmoYZ")
	{
		planeNormal.y = 0.0f;
		planeNormal.z = 0.0f;
	}
	else if (selectedGizmoName == "GizmoZ")
	{
		planeNormal.z = 0.0f;
	}

	if (editorLocalClient->showDebugPrimitives)
	{
		drawDebugLine(gizmoOrigin, gizmoOrigin + planeNormal * 5.0f, { 0.0f, 1.0f, 1.0f, 1.0f }, 4.0f, false, 5.0f);
	}

	glm::vec3 newPosition = rayVsPlane(gizmoSelectPositionOrigin, planeNormal, rayOrigin, rayDirection);
	newPosition -= clickOffset;

	// drawDebugPlane(gizmoOrigin, planeNormal, newPosition, 5.0f, true);

	if (selectedGizmoName == "GizmoX")
	{
		newPosition.y = gizmoOrigin.y;
		newPosition.z = gizmoOrigin.z;
	}
	else if (selectedGizmoName == "GizmoXY")
	{
		newPosition.z = gizmoOrigin.z;
	}
	else if (selectedGizmoName == "GizmoXZ")
	{
		newPosition.y = gizmoOrigin.y;
	}
	else if (selectedGizmoName == "GizmoY")
	{
		newPosition.x = gizmoOrigin.x;
		newPosition.z = gizmoOrigin.z;
	}
	else if (selectedGizmoName == "GizmoYZ")
	{
		newPosition.x = gizmoOrigin.x;
	}
	else if (selectedGizmoName == "GizmoZ")
	{
		newPosition.x = gizmoOrigin.x;
		newPosition.y = gizmoOrigin.y;
	}

	if (Entity* selection = editorLocalClient->getSelectedEntity())
	{
		if (auto transformComponent = selection->findComponent<TransformComponent>())
		{
			transformComponent->setPosition(newPosition);
		}
	}
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
			const std::string selectedGizmoName = selectedGizmoModel->getObjectName();

			const glm::vec3 rayOrigin = camera->getDerivedPosition();
			const glm::vec3 rayDirection = glm::normalize(localClient->getLocalServer()->getCursorToWorldRay(camera->getView(), camera->getProjection()));

			const glm::vec3 rayVsGizmo = glm::normalize(rayOrigin - selectedGizmoModel->getDerivedPosition());

			glm::vec3 newPosition = rayVsPlane(selectedGizmoModel->getDerivedPosition(), getSelectedGimbalUp(), rayOrigin, rayDirection);

			float positionAngle = getPositionAngle(selectedGizmoModel->getDerivedPosition(), positionRelativeToGimbal(newPosition));

			Euler selectedEuler = getSelectedGimbalEulerAngle();
			float newAngle = gizmoSelectRotationOrigin.get(selectedEuler) + positionAngle - clickRotator;

			selectionTransform->setRotation(newAngle, selectedEuler);
			selectedGizmoModel->setRotation(newAngle, selectedEuler);
		}
	}
}

float EditorController::getPositionAngle(const glm::vec3& center, const glm::vec2& position)
{
	float yaw = glm::degrees(glm::atan(position.x, position.y));
	return Rotator::normalizeAngle(yaw);
}

Euler EditorController::getSelectedGimbalEulerAngle()
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

// TODO: ALL DIRECTIONS MUST BE RELATIVE TO THE REST
glm::vec3 EditorController::getSelectedGimbalUp()
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

glm::vec2 EditorController::positionRelativeToGimbal(const glm::vec3& position)
{
	glm::vec3 norm = glm::normalize(position - selectedGizmoModel->getDerivedPosition());

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
