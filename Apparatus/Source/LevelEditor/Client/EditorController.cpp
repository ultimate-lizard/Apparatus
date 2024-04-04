#include "EditorController.h"

#include <glm/gtc/quaternion.hpp>

#include <Apparatus/Apparatus.h>
#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Components/CameraComponent.h>
#include <Apparatus/Components/MovementComponent.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Util/DebugPrimitive.h>

#include "EditorLocalClient.h"
#include "../Components/SelectableComponent.h"
#include "../Components/GizmoComponent.h"

EditorController::EditorController(EditorLocalClient* editorLocalClient) :
	HumanControllerBase(editorLocalClient),
	editorLocalClient(editorLocalClient)
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
	
	if (editorLocalClient)
	{
		if (Apparatus* apparatus = localClient->getApparatus())
		{
			apparatus->setCursorVisibleEnabled(true);
		}

		gizmo = editorLocalClient->getGizmo();
	}
}

void EditorController::onDeactivate()
{
	if (gizmo)
	{
		if (gizmo->isPressed())
		{
			releasePrimaryMouseButton();
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
	inputHandler->bindKeyAction("EnableScaleMode", KeyEventType::Press, std::bind(&EditorController::enableScaleMode, this));

	inputHandler->bindAxisAction("LookY", std::bind(&EditorController::cursorMoveY, this, std::placeholders::_1));
	inputHandler->bindAxisAction("LookX", std::bind(&EditorController::cursorMoveX, this, std::placeholders::_1));
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

	bool foundGizmo = false;
	for (const RayTraceResult& result : traceResults)
	{
		if (result.entity->getObjectName() == GizmoNames::Gizmo)
		{
			foundGizmo = true;

			if (gizmo)
			{
				gizmo->setSelectedGizmoModel(result.modelComponent);
				gizmo->press(result.near);
			}

			break;
		}
	}

	if (!foundGizmo)
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
	if (gizmo)
	{
		gizmo->setInteractionMode(InteractionMode::Translation);
	}
}

void EditorController::enableRotationMode()
{
	if (gizmo)
	{
		gizmo->setInteractionMode(InteractionMode::Rotation);
	}
}

void EditorController::enableScaleMode()
{
	if (gizmo)
	{
		gizmo->setInteractionMode(InteractionMode::Scale);
	}
}

void EditorController::releasePrimaryMouseButton()
{
	if (editorLocalClient)
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

	if (gizmo)
	{
		gizmo->release();
	}
}

void EditorController::cursorMoveY(float value)
{
	if (gizmo)
	{
		if (gizmo->isPressed())
		{
			gizmo->handleCursorMovement(0.0f, value);
		}
	}
}

void EditorController::cursorMoveX(float value)
{
	if (gizmo)
	{
		if (gizmo->isPressed())
		{
			gizmo->handleCursorMovement(value, 0.0f);
		}
	}
}
