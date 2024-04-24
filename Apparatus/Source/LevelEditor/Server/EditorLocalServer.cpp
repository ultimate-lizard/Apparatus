#include "EditorLocalServer.h"

#include <Apparatus/Level.h>
#include <Apparatus/Util/BoundingBox.h>
#include <Apparatus/Rendering/Renderer.h>
#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/LightComponent.h>
#include "../Components/SelectableComponent.h"
#include "../Components/GizmoComponent.h"
#include "../Editor/Editor.h"

EditorLocalServer::EditorLocalServer() :
	LocalServer(),
	gizmo(nullptr),
	selectionProxyTransformComponent(nullptr),
	snapToGrid(false),
	shiftPressed(false)
{
}

void EditorLocalServer::connect(const ConnectionInfo& info)
{
	LocalServer::connect(info);


}

void EditorLocalServer::update(float dt)
{
	LocalServer::update(dt);
}

void EditorLocalServer::start()
{
	LocalServer::start();

	if (level)
	{
		if (Entity* selectionProxy = level->spawnEntity("SelectionProxy"))
		{
			selectionProxyTransformComponent = selectionProxy->findComponent<TransformComponent>();

			if (ModelComponent* proxyModelComponent = selectionProxy->findComponent<ModelComponent>())
			{
				proxyModelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleAll"));
			}
		}

		if (Entity* player = level->spawnEntity("Player"))
		{
			if (TransformComponent* transformComponent = player->findComponent<TransformComponent>())
			{
				//transformComponent->rotate(180.0f, Euler::Yaw);
			}
		}

		if (Entity* gizmo = level->spawnEntity("Gizmo"))
		{
			this->gizmo = gizmo->findComponent<GizmoComponent>();
		}

		if (Entity* sceneModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = sceneModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_NewScene"));
			}
		}

		if (Entity* dungeonModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = dungeonModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_CaveWall"));
			}

			if (TransformComponent* transform = dungeonModel->findComponent<TransformComponent>())
			{
				transform->setPosition({ -2.0f, 1.0f, 0.0f });
			}
		}

		if (Entity* dungeonModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = dungeonModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_CellDoor"));
			}

			if (TransformComponent* transform = dungeonModel->findComponent<TransformComponent>())
			{
				transform->setPosition({ -1.0f, 1.0f, 0.0f });
			}
		}

		if (Entity* dungeonModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = dungeonModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_Table"));
			}

			if (TransformComponent* transform = dungeonModel->findComponent<TransformComponent>())
			{
				transform->setPosition({ 0.0f, 1.0f, 0.0f });
			}
		}

		if (Entity* dungeonModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = dungeonModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_Shrine"));
			}

			if (TransformComponent* transform = dungeonModel->findComponent<TransformComponent>())
			{
				transform->setPosition({ 1.0f, 1.0f, 0.0f });
			}
		}

		if (Entity* dungeonModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = dungeonModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_Cage"));
			}

			if (TransformComponent* transform = dungeonModel->findComponent<TransformComponent>())
			{
				transform->setPosition({ 2.0f, 1.0f, 0.0f });
			}
		}

		if (Entity* dungeonModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = dungeonModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_LightPost"));
			}

			if (TransformComponent* transform = dungeonModel->findComponent<TransformComponent>())
			{
				transform->setPosition({ 3.0f, 1.0f, 0.0f });
			}
		}

		if (Entity* dungeonModel = level->spawnEntity("ModelEntity"))
		{
			if (ModelComponent* modelComponent = dungeonModel->findComponent<ModelComponent>())
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_Chair"));
			}

			if (TransformComponent* transform = dungeonModel->findComponent<TransformComponent>())
			{
				transform->setPosition({ 4.0f, 1.0f, 0.0f });
			}
		}

		if (Entity* spotLight = level->spawnEntity("SpotLight"))
		{
			if (LightComponent* pointLightComponent = spotLight->findComponent<LightComponent>())
			{
				pointLightComponent->setColor({ 1.0f, 0.0f, 0.0f });
				pointLightComponent->setRadius(20.0f);
			}
		}

		if (Entity* directionalLight = level->spawnEntity("DirectionalLight"))
		{
			if (LightComponent* pointLightComponent = directionalLight->findComponent<LightComponent>())
			{
				pointLightComponent->setColor({ 0.0f, 0.0f, 0.1f });
			}
		}

		if (Entity* pointLight = level->spawnEntity("PointLight"))
		{
			if (LightComponent* pointLightComponent = pointLight->findComponent<LightComponent>())
			{
				pointLightComponent->setColor({ 1.0f, 0.8f, 0.4f });
			}
		}
	}
}

void EditorLocalServer::selectEntity(Entity* entity)
{
	if (!selectionProxyTransformComponent)
	{
		return;
	}

	Entity* entityToSelect = entity;

	if (!isShiftPressed())
	{
		for (TransformComponent* selectedTransformComponent : selectedTransformComponents)
		{
			indicateSelection(selectedTransformComponent->getOwner(), false);
		}

		applyMultiselectTransform();
		resetProxyTransform();

		selectedTransformComponents.clear();
	}

	if (entityToSelect)
	{
		indicateSelection(entityToSelect, true);

		if (TransformComponent* transformComponent = entityToSelect->findComponent<TransformComponent>())
		{
			selectedTransformComponents.push_back(transformComponent);
		}
	}

	if (isShiftPressed())
	{
		entityToSelect = selectionProxyTransformComponent->getOwner();

		applyMultiselectTransform();
		resetProxyTransform();
		selectionProxyTransformComponent->setPosition(calculateAverageSelectionPosition());

		for (TransformComponent* selectedTransformComponent : selectedTransformComponents)
		{
			selectedTransformComponent->setParent(selectionProxyTransformComponent);
			selectedTransformComponent->setPosition(selectedTransformComponent->getPosition() - selectionProxyTransformComponent->getWorldPosition());
		}
	}

	if (gizmo)
	{
		gizmo->setSelectedEntity(entityToSelect);
		gizmo->attach(entityToSelect);
		gizmo->setVisibility(entityToSelect != nullptr);
	}
}

void EditorLocalServer::duplicateSelection()
{
	std::vector<TransformComponent*> selections = selectedTransformComponents;

	selectEntity(nullptr);

	for (TransformComponent* selectedTransformComponent : selections)
	{
		Entity* selectedEntity = selectedTransformComponent->getOwner();

		if (Entity* newSelection = Apparatus::getEntityRegistry().clone(selectedEntity))
		{
			newSelection->init();
			newSelection->onSpawn();

			if (level)
			{
				level->addEntity(newSelection);
			}

			bool shiftWasPressed = isShiftPressed();

			if (!shiftWasPressed)
			{
				setShiftPressed(true);
			}

			selectEntity(newSelection);

			if (!shiftWasPressed)
			{
				setShiftPressed(false);
			}
		}
	}
}

void EditorLocalServer::setSnapToGridEnabled(bool enabled)
{
	snapToGrid = enabled;
	if (gizmo)
	{
		gizmo->setSnapToGridEnabled(enabled);
	}
}

bool EditorLocalServer::isSnapToGridEnabled()
{
	return snapToGrid == true;
}

void EditorLocalServer::indicateSelection(Entity* entity, bool selected)
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
}

void EditorLocalServer::applyMultiselectTransform()
{
	for (TransformComponent* selectedTransformComponent : selectedTransformComponents)
	{
		// Adding a new entity to the selection list. Apply all changes
		selectedTransformComponent->setPosition(selectedTransformComponent->getWorldPosition());
		selectedTransformComponent->setScale(selectedTransformComponent->getWorldScale());
		selectedTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Pitch), Euler::Pitch);
		selectedTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Yaw), Euler::Yaw);
		selectedTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Roll), Euler::Roll);

		selectedTransformComponent->setParent(nullptr);
	}
}

void EditorLocalServer::resetProxyTransform()
{
	selectionProxyTransformComponent->setRotation(Rotator());
	selectionProxyTransformComponent->setScale(glm::vec3(1.0f));
	selectionProxyTransformComponent->setPosition(glm::vec3(0.0f));
}

glm::vec3 EditorLocalServer::calculateAverageSelectionPosition()
{
	glm::vec3 averagePosition(0.0f);
	size_t transformCount = 0;

	for (TransformComponent* selectedTransformComponent : selectedTransformComponents)
	{
		averagePosition += selectedTransformComponent->getWorldPosition();
		transformCount++;
	}

	averagePosition /= transformCount;

	return averagePosition;
}

void EditorLocalServer::setShiftPressed(bool pressed)
{
	shiftPressed = pressed;
}

bool EditorLocalServer::isShiftPressed() const
{
	return shiftPressed == true;
}
