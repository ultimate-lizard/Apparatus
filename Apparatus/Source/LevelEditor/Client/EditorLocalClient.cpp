#include "EditorLocalClient.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Core/Logger.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>

#include "EditorController.h"
#include "../Components/SelectableComponent.h"
#include "../Components/GizmoComponent.h"

EditorLocalClient::EditorLocalClient(Apparatus* apparatus) :
	LocalClient(apparatus),
	inEditMode(false),
	selectedEntity(nullptr),
	gizmo(nullptr)
{
	assignDefaultObjectName();
}

void EditorLocalClient::selectEntity(Entity* entity)
{
	indicateSelection(selectedEntity, false);

	this->selectedEntity = entity;

	if (entity)
	{
		indicateSelection(entity, true);
	}

	if (gizmo)
	{
		gizmo->attach(entity);
		gizmo->setVisibility(entity != nullptr);
	}
}

Entity* EditorLocalClient::getSelectedEntity()
{
	return selectedEntity;
}

GizmoComponent* EditorLocalClient::getGizmo()
{
	return gizmo;
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

	if (localServer)
	{
		if (Entity* gizmoEntity = localServer->findEntity(GizmoNames::Gizmo))
		{
			gizmo = gizmoEntity->findComponent<GizmoComponent>();
			gizmo->setEditorLocalClient(this);
		}
	}

	if (gizmo)
	{
		gizmo->setVisibility(false);
	}
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
