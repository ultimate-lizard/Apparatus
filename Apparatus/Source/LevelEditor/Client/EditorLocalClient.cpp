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
			if (modelComponent->getObjectName().find("Gizmo") != std::string::npos)
			{
				modelComponent->setVisibility(enabled);
			}
			else
			{
				modelComponent->setVisibility(false);
			}

			//if (selectedEntity)
			//{
			//	if (auto selectionTransform = selectedEntity->findComponent<TransformComponent>())
			//	{
			//		// TRANSLATION
			//		std::vector<ModelComponent*> modelComponents = selectedEntity->getAllComponentsOfClass<ModelComponent>();
			//		for (ModelComponent* modelComponent : modelComponents)
			//		{
			//			if (!modelComponent)
			//			{
			//				continue;
			//			}

			//			const std::string& name = modelComponent->getObjectName();
			//			if (name != "Pitch" && name != "Yaw" && name != "Roll")
			//			{
			//				if (SceneNode* parentOfSelection = selectionTransform->getParent())
			//				{
			//					// modelComponent->setPosition(parentOfSelection->getDerivedOrientation() * selectionTransform->getPosition());
			//				}
			//			}
			//		}
			//	}
			//}
		}
		else if (interactionMode == InteractionMode::Rotation)
		{
			std::string modelName = modelComponent->getObjectName();
			if (modelName == "Pitch" || modelName == "Yaw" || modelName == "Roll")
			{
				modelComponent->setVisibility(enabled);
			}
			else
			{
				modelComponent->setVisibility(false);
			}

			if (!selectedEntity)
			{
				continue;
			}
		}
	}
}

void EditorLocalClient::attachGizmo(Entity* entityToAttachTo)
{
	if (!gizmo)
	{
		return;
	}

	if (auto gizmoTransform = gizmo->findComponent<TransformComponent>())
	{
		if (entityToAttachTo)
		{
			if (auto entityTransform = entityToAttachTo->findComponent<TransformComponent>())
			{
				// Attach gizmo to entity (gizmo doesn't inherit rotation)
				gizmoTransform->setPosition(glm::vec3(0.0f));
				gizmoTransform->setParent(entityTransform);

				// Gizmo must inherit entity parent's rotation manually, or reset its rotation, if no parent.
				// After that, gizmos and gimbals will face parent's direction but would still have their own local rotations
				if (SceneNode* entityParent = entityTransform->getParent())
				{
					// Gizmo now in the parent's space
					gizmoTransform->setOrientation(entityParent->getDerivedOrientation());
				}
				else
				{
					// Gizmo now in the global space
					gizmoTransform->setOrientation(glm::quat(glm::vec3(0.0f)));
				}
			}
		}
		else
		{
			gizmoTransform->setParent(nullptr);
		}
	}

	// Pass the selected entity's LOCAL rotations to the gimbals
	if (auto selectionTransform = selectedEntity->findComponent<TransformComponent>())
	{
		// ROTATION
		const float pitch = selectionTransform->getRotationAngle(Euler::Pitch);
		const float yaw = selectionTransform->getRotationAngle(Euler::Yaw);
		const float roll = selectionTransform->getRotationAngle(Euler::Roll);

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
