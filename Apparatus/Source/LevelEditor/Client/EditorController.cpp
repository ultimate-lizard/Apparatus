#include "EditorController.h"

#include <glm/gtc/quaternion.hpp>

#include <Apparatus/Apparatus.h>
#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Components/CameraComponent.h>
#include <Apparatus/Components/MovementComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Util/DebugPrimitive.h>
#include <Apparatus/Util/CollisionDetection.h>
#include <Apparatus/Level.h>

#include "EditorLocalClient.h"
#include "../Components/SelectableComponent.h"
#include "../Components/GizmoComponent.h"
#include "../Server/EditorLocalServer.h"
#include "../Editor/Editor.h"

EditorController::EditorController(const std::string& controllerName, LocalClient& localClient) :
	HumanControllerBase(controllerName, localClient),
	gizmo(nullptr),
	ctrlPressed(false)
{
}

void EditorController::onActivate()
{
	Apparatus& app = Apparatus::get();
	app.getWindow().setCursorVisibleEnabled(true);

	if (Level* level = app.getLevel())
	{
		if (Entity* gizmoEntity = level->findEntity("Gizmo"))
		{
			gizmo = gizmoEntity->findComponent<GizmoComponent>();
		}
	}
}

void EditorController::onDeactivate()
{
	if (gizmo)
	{
		if (gizmo->isPressed())
		{
			releaseSelect();
		}
	}
}

void EditorController::setupInput()
{
	InputHandler& inputHandler = localClient.getInputHandler();

	inputHandler.bindKeyAction("Fire", KeyEventType::Press, std::bind(&EditorController::pressSelect, this));
	inputHandler.bindKeyAction("Fire", KeyEventType::Release, std::bind(&EditorController::releaseSelect, this));

	inputHandler.bindKeyAction("AltFire", KeyEventType::Press, std::bind(&EditorController::cancelSelection, this));
	inputHandler.bindKeyAction("OpenMainMenu", KeyEventType::Press, std::bind(&EditorController::cancelSelection, this));

	inputHandler.bindKeyAction("EnableTranslationMode", KeyEventType::Press, std::bind(&EditorController::enableTranslationMode, this));
	inputHandler.bindKeyAction("EnableRotationMode", KeyEventType::Press, std::bind(&EditorController::enableRotationMode, this));
	inputHandler.bindKeyAction("EnableScaleMode", KeyEventType::Press, std::bind(&EditorController::enableScaleMode, this));

	inputHandler.bindAxisAction("LookY", std::bind(&EditorController::cursorMoveY, this, std::placeholders::_1));
	inputHandler.bindAxisAction("LookX", std::bind(&EditorController::cursorMoveX, this, std::placeholders::_1));

	inputHandler.bindKeyAction("EditorModifier", KeyEventType::Press, std::bind(&EditorController::onModifierPressed, this));
	inputHandler.bindKeyAction("EditorModifier", KeyEventType::Release, std::bind(&EditorController::onModifierReleased, this));

	inputHandler.bindKeyAction("Duplicate", KeyEventType::Press, std::bind(&EditorController::onDuplicate, this));
	inputHandler.bindKeyAction("ToggleSnapToGrid", KeyEventType::Press, std::bind(&EditorController::toggleSnapToGrid, this));

	inputHandler.bindKeyAction("Shift", KeyEventType::Press, std::bind(&EditorController::onShiftPressed, this));
	inputHandler.bindKeyAction("Shift", KeyEventType::Release, std::bind(&EditorController::onShiftReleased, this));
}

void EditorController::pressSelect()
{
	Apparatus& app = Apparatus::get();
	
	LocalClient* localClient = app.getPrimaryLocalClient();
	if (!localClient)
	{
		return;
	}

	Camera* camera = localClient->getActiveCamera();
	if (!camera)
	{
		return;
	}

	glm::vec3 direction = getCursorToWorldRay(camera->getView(), camera->getProjection());
	glm::vec3 origin = camera->getWorldPosition();

	if (Level* level = app.getLevel())
	{
		std::vector<RayTraceResult> traceResults = level->traceRay(origin, direction, DetectionType::Visibility);

		bool foundGizmo = false;
		for (const RayTraceResult& result : traceResults)
		{
			if (result.entity->getEntityName() == GizmoNames::Gizmo)
			{
				foundGizmo = true;

				if (const ModelComponent* gizmoModelComponent = result.modelComponent)
				{
					if (Entity* gizmo = result.entity)
					{
						if (GizmoComponent* gizmoComponent = gizmo->findComponent<GizmoComponent>())
						{
							gizmoComponent->setSelectedGizmoModel(gizmoModelComponent->getComponentName());
							gizmoComponent->press(result.near);
						}
					}
				}

				break;
			}
		}

		if (!foundGizmo)
		{
			if (Editor* editor = Apparatus::getAs<Editor>())
			{
				if (EditorLocalServer* editorLocalServer = editor->getServer<EditorLocalServer>())
				{
					// Select the closest entity to the ray origin. Index 0 is the closest
					editorLocalServer->selectEntity(!traceResults.empty() ? traceResults[0].entity : nullptr);
				}
			}
		}
	}
}

void EditorController::cancelSelection()
{
	if (Editor* editor = Apparatus::getAs<Editor>())
	{
		if (EditorLocalServer* editorLocalServer = editor->getServer<EditorLocalServer>())
		{
			// Select the closest entity to the ray origin. Index 0 is the closest
			editorLocalServer->selectEntity(nullptr);
		}
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

void EditorController::releaseSelect()
{
	//if (editorLocalClient)
	//{
	//	if (Entity* selectedEntity = editorLocalClient->getSelectedEntity())
	//	{
	//		if (auto selectableComponent = selectedEntity->findComponent<SelectableComponent>())
	//		{
	//			selectableComponent->regenerateVisualBoundingBox();
	//			selectableComponent->setBoxVisible(true);
	//		}
	//	}
	//}

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

void EditorController::onModifierPressed()
{
	ctrlPressed = true;
}

void EditorController::onModifierReleased()
{
	ctrlPressed = false;
}

void EditorController::onDuplicate()
{
	if (!ctrlPressed)
	{
		return;
	}

	Apparatus& app = Apparatus::get();
	if (EditorLocalServer* editorLocalServer = app.getServer<EditorLocalServer>())
	{
		editorLocalServer->duplicateSelection();
	}
}

void EditorController::toggleSnapToGrid()
{
	Apparatus& app = Apparatus::get();
	if (EditorLocalServer* editorLocalServer = app.getServer<EditorLocalServer>())
	{
		editorLocalServer->setSnapToGridEnabled(!editorLocalServer->isSnapToGridEnabled());
	}
}

void EditorController::onShiftPressed()
{
	Apparatus& app = Apparatus::get();
	if (EditorLocalServer* editorLocalServer = app.getServer<EditorLocalServer>())
	{
		editorLocalServer->setShiftPressed(true);
	}
}

void EditorController::onShiftReleased()
{
	Apparatus& app = Apparatus::get();
	if (EditorLocalServer* editorLocalServer = app.getServer<EditorLocalServer>())
	{
		editorLocalServer->setShiftPressed(false);
	}
}
