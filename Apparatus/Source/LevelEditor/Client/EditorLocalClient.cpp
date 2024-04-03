#include "EditorLocalClient.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Core/Logger.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>

#include "EditorController.h"
#include "../Components/SelectableComponent.h"

EditorLocalClient::EditorLocalClient(Apparatus* apparatus) :
	LocalClient(apparatus),
	inEditMode(false),
	selectedEntity(nullptr),
	gizmo(nullptr),
	interactionMode(InteractionMode::Translation)
{
	assignDefaultObjectName();
}

void EditorLocalClient::setGizmoVisibility(bool enabled)
{
	if (!gizmo)
	{
		return;
	}

	for (ModelComponent* modelComponent : gizmo->getAllComponentsOfClass<ModelComponent>())
	{
		if (!modelComponent)
		{
			continue;
		}

		if (interactionMode == InteractionMode::Translation)
		{
			modelComponent->setVisibility((modelComponent->getObjectName().find("Gizmo") != std::string::npos) && enabled);
		}
		else if (interactionMode == InteractionMode::Rotation)
		{
			std::string modelName = modelComponent->getObjectName();
			modelComponent->setVisibility((modelName == "Pitch" || modelName == "Yaw" || modelName == "Roll") && enabled);
		}
	}
}

void EditorLocalClient::attachGizmo(Entity* entityToAttachTo)
{
	if (!gizmo)
	{
		return;
	}

	TransformComponent* gizmoTransform = gizmo->findComponent<TransformComponent>();
	if (!gizmoTransform)
	{
		return;
	}

	if (!entityToAttachTo)
	{
		gizmoTransform->setParent(nullptr);
		gizmoTransform->setOrientation(glm::quat(glm::vec3(0.0f)));
		gizmoTransform->setPosition(glm::vec3(0.0f));

		return;
	}

	if (TransformComponent* entityTransform = entityToAttachTo->findComponent<TransformComponent>())
	{
		// POSITION -------------------------------------------------------
		// Set local position of gizmo to 0 so it will not look displaced relative to the parent
		gizmoTransform->setPosition(glm::vec3(0.0f));
		// Inherit position of the entity. Gizmo doesn't inherit rotation, though. It is set manually
		gizmoTransform->setParent(entityTransform);

		// Gizmo must inherit entity parent's rotation manually, or reset its rotation, if no parent.
		// After that, gizmos and gimbals will face parent's direction but still have their own local rotations
		if (SceneNode* entityParent = entityTransform->getParent())
		{
			// Gizmo rotation is now in the parent space
			gizmoTransform->setOrientation(entityParent->getDerivedOrientation());
		}
		else
		{
			// Gizmo rotation is not in the global space
			gizmoTransform->setOrientation(glm::quat(glm::vec3(0.0f)));
		}

		// ROTATION -------------------------------------------------------
		// Pass the selected entity's LOCAL rotations to the gimbals
		const float pitch = entityTransform->getRotationAngle(Euler::Pitch);
		const float yaw = entityTransform->getRotationAngle(Euler::Yaw);
		const float roll = entityTransform->getRotationAngle(Euler::Roll);

		if (auto modelComponent = gizmo->findComponent<ModelComponent>("Pitch"))
		{
			modelComponent->setRotation(pitch, Euler::Pitch);
		}

		if (auto modelComponent = gizmo->findComponent<ModelComponent>("Yaw"))
		{
			modelComponent->setRotation(yaw, Euler::Yaw);
		}

		if (auto modelComponent = gizmo->findComponent<ModelComponent>("Roll"))
		{
			modelComponent->setRotation(roll, Euler::Roll);
		}
	}
}

void EditorLocalClient::selectEntity(Entity* entity)
{
	indicateSelection(selectedEntity, false);

	this->selectedEntity = entity;

	if (entity)
	{
		indicateSelection(entity, true);
		attachGizmo(entity);
		setGizmoVisibility(true);
	}

	if (!entity)
	{
		setGizmoVisibility(false);
	}
}

Entity* EditorLocalClient::getSelectedEntity()
{
	return selectedEntity;
}

void EditorLocalClient::indicateSelection(Entity* entity, bool selected)
{
	if (!entity)
	{
		return;
	}

	for (ModelComponent* modelComponent : entity->getAllComponentsOfClass<ModelComponent>())
	{
		if (!modelComponent)
		{
			continue;
		}

		if (ModelInstance* modelInstance = modelComponent->getModelInstance())
		{
			if (Model* model = modelInstance->getModel())
			{
				const size_t materialsNum = model->getMaterials().size();
				for (size_t i = 0; i < materialsNum; ++i)
				{
					if (MaterialInstance* materialInstance = modelInstance->getMaterialInstance(i))
					{
						MaterialParameters& params = materialInstance->getMaterialParameters();

						params.setVec4("selectionColor", { 1.0f, 0.0f, 0.0f, 1.0f });
						params.setBool("selected", selected);
					}
				}
			}
		}
	}

	if (auto selectableComponent = entity->findComponent<SelectableComponent>())
	{
		selectableComponent->setSelected(selected);
		selectableComponent->regenerateVisualBoundingBox();
	}
}

InteractionMode EditorLocalClient::getInteractionMode() const
{
	return interactionMode;
}

void EditorLocalClient::setInteractionMode(InteractionMode interactionMode)
{
	this->interactionMode = interactionMode;
	setGizmoVisibility(getSelectedEntity() != nullptr);
}

void EditorLocalClient::assignDefaultObjectName()
{
	setObjectName("EditorLocalClient");
}

void EditorLocalClient::init()
{
	LocalClient::init();

	createController<EditorController>(this);

	setupGlobalEditorInput();
}

void EditorLocalClient::start()
{
	LocalClient::start();

	gizmo = localServer->findEntity("Gizmo");
	setGizmoVisibility(false);
}

void EditorLocalClient::onActiveControllerChanged()
{
	setupGlobalEditorInput();
}

void EditorLocalClient::toggleEditMode()
{
	setEditModeEnabled(!isInEditMode());
}

void EditorLocalClient::setEditModeEnabled(bool enabled)
{
	inEditMode = enabled;
	if (apparatus)
	{
		apparatus->setCursorWrapEnabled(!enabled);
	}
	
	if (inEditMode)
	{
		setActiveController(findController<EditorController>("EditorController"));
		if (Entity* playerEntity = localServer->findEntity("Player"))
		{
			setActiveEntity(playerEntity);
		}
	}
	else
	{
		setActiveController(getDefaultController());
	}

	if (Entity* playerEntity = localServer->findEntity("Player"))
	{
		setActiveEntity(playerEntity);
	}

	if (Controller* activeController = getActiveController())
	{
		activeController->setControlEntity(getActiveEntity());
	}

	// TODO: Replace with UI
	LOG(enabled ? "Edit mode" : "Free camera mode", LogLevel::Info);
}

bool EditorLocalClient::isInEditMode() const
{
	return inEditMode == true;
}

void EditorLocalClient::setupGlobalEditorInput()
{
	getInputHandler().bindKeyAction("ToggleEditMode", KeyEventType::Press, std::bind(&EditorLocalClient::toggleEditMode, this));
}
