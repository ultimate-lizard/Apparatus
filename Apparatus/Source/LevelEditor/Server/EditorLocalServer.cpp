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
	if (!isShiftPressed())
	{
		for (Entity* selectedEntity : selectedEntities)
		{
			indicateSelection(selectedEntity, false);

			if (TransformComponent* selectionTransformComponent = selectedEntity->findComponent<TransformComponent>())
			{
				selectionTransformComponent->setPosition(selectionTransformComponent->getWorldPosition());

				selectionTransformComponent->setParent(nullptr);

				selectionTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Pitch), Euler::Pitch);
				selectionTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Yaw), Euler::Yaw);
				selectionTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Roll), Euler::Roll);
			}
		}

		selectionProxyTransformComponent->setRotation(Rotator());
		selectionProxyTransformComponent->setPosition(glm::vec3(0.0f));

		selectedEntities.clear();
	}

	if (entity)
	{
		selectedEntities.push_back(entity);
	}

	if (entity)
	{
		indicateSelection(entity, true);
	}

	if (gizmo)
	{
		if (isShiftPressed())
		{
			for (Entity* selectedEntity : selectedEntities)
			{
				if (TransformComponent* selectionTransformComponent = selectedEntity->findComponent<TransformComponent>())
				{
					selectionTransformComponent->setPosition(selectionTransformComponent->getWorldPosition());

					selectionTransformComponent->setParent(nullptr);

					selectionTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Pitch), Euler::Pitch);
					selectionTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Yaw), Euler::Yaw);
					selectionTransformComponent->rotate(selectionProxyTransformComponent->getRotationAngle(Euler::Roll), Euler::Roll);
				}
			}

			glm::vec3 averagePosition(0.0f);
			size_t transformCount = 0;

			glm::vec3 averageRotator(0.0f);

			for (Entity* selectedEntity : selectedEntities)
			{
				if (TransformComponent* selectionTransformComponent = selectedEntity->findComponent<TransformComponent>())
				{
					averagePosition += selectionTransformComponent->getWorldPosition();

					Rotator rot = selectionTransformComponent->getRotator();
					averageRotator.x = rot.get(Euler::Pitch);
					averageRotator.y = rot.get(Euler::Yaw);
					averageRotator.z = rot.get(Euler::Roll);

					transformCount++;
				}
			}

			averagePosition /= transformCount;

			selectionProxyTransformComponent->setPosition(averagePosition);
			selectionProxyTransformComponent->setRotation(Rotator());

			Rotator newRotator(averageRotator.x, averageRotator.y, averageRotator.z);

			for (Entity* selectedEntity : selectedEntities)
			{
				if (TransformComponent* selectionTransformComponent = selectedEntity->findComponent<TransformComponent>())
				{
					selectionTransformComponent->setParent(selectionProxyTransformComponent);
					selectionTransformComponent->setPosition(selectionTransformComponent->getPosition() - selectionProxyTransformComponent->getWorldPosition());
				}
			}

			gizmo->setSelectedEntity(selectionProxyTransformComponent->getOwner());
			gizmo->attach(selectionProxyTransformComponent->getOwner());
			gizmo->setVisibility(selectionProxyTransformComponent->getOwner() != nullptr);
		}
		else
		{
			gizmo->setSelectedEntity(entity);
			gizmo->attach(entity);
			gizmo->setVisibility(entity != nullptr);
		}
	}
}

void EditorLocalServer::duplicateSelection()
{
	std::vector<Entity*> selections = selectedEntities;

	selectEntity(nullptr);

	for (Entity* selectedEntity : selections)
	{
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

void EditorLocalServer::setShiftPressed(bool pressed)
{
	shiftPressed = pressed;
}

bool EditorLocalServer::isShiftPressed() const
{
	return shiftPressed == true;
}
