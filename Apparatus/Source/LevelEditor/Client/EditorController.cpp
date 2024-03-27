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
	if (inputHandler)
	{
		inputHandler->bindKeyAction("Fire", KeyEventType::Press, std::bind(&EditorController::pressSelect, this));
		inputHandler->bindKeyAction("Fire", KeyEventType::Release, std::bind(&EditorController::releasePrimaryMouseButton, this));

		inputHandler->bindKeyAction("AltFire", KeyEventType::Press, std::bind(&EditorController::releaseSelect, this));
		inputHandler->bindKeyAction("OpenMainMenu", KeyEventType::Press, std::bind(&EditorController::releaseSelect, this));

		inputHandler->bindKeyAction("EnableTranslationMode", KeyEventType::Press, std::bind(&EditorController::enableTranslationMode, this));
		inputHandler->bindKeyAction("EnableRotationMode", KeyEventType::Press, std::bind(&EditorController::enableRotationMode, this));

		inputHandler->bindAxisAction("LookY", std::bind(&EditorController::cursorMoveY, this, std::placeholders::_1));
		inputHandler->bindAxisAction("LookX", std::bind(&EditorController::cursorMoveX, this, std::placeholders::_1));
	}
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

	const float selectRayLength = 50'000.0f;
	std::vector<RayTraceResult> traceResults = localServer->traceRay(origin, direction, selectRayLength, DetectionType::Visibility);

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

			break;
		}
	}

	if (gizmoTraced)
	{
		gizmoPressed = true;

		if (selectedGizmoModel)
		{
			gizmoSelectOrigin = selectedGizmoModel->getDerivedPosition();
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

	planeNormal = glm::normalize(gizmoOrigin - rayOrigin);
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

	glm::vec3 newPosition = rayVsPlane(gizmoSelectOrigin, planeNormal, rayOrigin, rayDirection);
	newPosition -= clickOffset;

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

	if (Entity* selection = editorLocalClient->getSelectedEntity())
	{
		if (auto transformComponent = selection->findComponent<TransformComponent>())
		{
			const std::string selectedGizmoName = selectedGizmoModel->getObjectName();
			if (selectedGizmoName == "Pitch")
			{
				transformComponent->rotate(mouseInputX, Euler::Pitch);
				selectedGizmoModel->rotate(mouseInputX, Euler::Pitch);
			}
			else if (selectedGizmoName == "Yaw")
			{
				transformComponent->rotate(mouseInputX + mouseInputY, Euler::Yaw);
				selectedGizmoModel->rotate(mouseInputX + mouseInputY, Euler::Yaw);
			}
			else if (selectedGizmoName == "Roll")
			{
				transformComponent->rotate(mouseInputX, Euler::Roll);
				selectedGizmoModel->rotate(mouseInputX, Euler::Roll);
			}
		}
	}
}
