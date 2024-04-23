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
	// selectedEntity(nullptr),
	gizmo(nullptr),
	selectionProxyTransformComponent(nullptr),
	cachedSelectionPosition(0.0f),
	selectionBoxVisible(false),
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

	/*if (selectedEntity)
	{
		if (auto transform = selectedEntity->findComponent<TransformComponent>())
		{
			if (transform->getWorldPosition() != cachedSelectionPosition)
			{
				glm::vec3 delta = transform->getWorldPosition() - cachedSelectionPosition;
				cachedSelectionPosition = transform->getWorldPosition();
				selectionBox.position += delta;
			}
		}

		if (isSelectionBoxVisible())
		{
			drawDebugBox(selectionBox, { 1.0f, 1.0f, 0.0f, 1.0f }, 1.0f);
		}
	}*/
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
		// regenerateSelectionBoundingBox();
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
//
//Entity* EditorLocalServer::getSelectedEntity()
//{
//	return selectedEntity;
//}

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

void EditorLocalServer::setSelectionBoxVisibility(bool visible)
{
	selectionBoxVisible = visible;
}

bool EditorLocalServer::isSelectionBoxVisible()
{
	return selectionBoxVisible == true;
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

	//if (auto selectableComponent = entity->findComponent<SelectableComponent>())
	//{
	//	selectableComponent->setSelected(selected);
	//	selectableComponent->regenerateVisualBoundingBox();
	//}
}

void EditorLocalServer::regenerateSelectionBoundingBox()
{
	/*if (!selectedEntity)
	{
		return;
	}

	auto transformComponent = selectedEntity->findComponent<TransformComponent>();
	if (!transformComponent)
	{
		return;
	}

	cachedSelectionPosition = transformComponent->getWorldPosition();

	std::vector<ModelComponent*> modelComponents = selectedEntity->getAllComponentsOfClass<ModelComponent>();
	if (modelComponents.size() > 1)
	{
		std::vector<Model*> models;
		for (ModelComponent* modelComponent : modelComponents)
		{
			if (modelComponent)
			{
				if (Model* model = modelComponent->getModel())
				{
					models.push_back(model);
				}
			}
		}

		selectionBox = generateAABB(models, transformComponent->getWorldPosition(), transformComponent->getWorldOrientation(), transformComponent->getWorldScale());
	}
	else if (!modelComponents.empty())
	{
		ModelComponent* modelComponent = modelComponents[0];
		selectionBox = generateAABB(modelComponent->getModel(), modelComponent->getWorldPosition(), modelComponent->getWorldOrientation(), modelComponent->getWorldScale());
	}*/
}

void EditorLocalServer::setShiftPressed(bool pressed)
{
	shiftPressed = pressed;
}

bool EditorLocalServer::isShiftPressed() const
{
	return shiftPressed == true;
}

//EditorContext& EditorLocalServer::getEditorContext()
//{
//	return editorContext;
//}

//
//void EditorLocalServer::setupEntities()
//{
//	LocalServer::setupEntities();
//
//	//if (Entity* modelEntity = createEntity("ModelEntity"))
//	//{
//	//	auto modelTransform = modelEntity->createComponent<TransformComponent>(modelEntity);
//	//	if (auto modelComponent = modelEntity->createComponent<ModelComponent>(modelEntity))
//	//	{
//	//		modelComponent->setModel(assetManager.findAsset<Model>("Model_Makarov"));
//	//		modelComponent->setParent(modelTransform);
//	//	}
//	//	modelTransform->setPosition({ 3.0f, 0.0f, 14.0f });
//	//	modelTransform->setScale(glm::vec3(1.5f));
//	//	
//	//	if (Entity* anotherEntity = createEntity("ModelEntity2"))
//	//	{
//	//		auto anotherEntityTransform = anotherEntity->createComponent<TransformComponent>(anotherEntity);
//	//		anotherEntityTransform->setParent(modelTransform);
//	//		if (auto modelComponent = anotherEntity->createComponent<ModelComponent>(anotherEntity))
//	//		{
//	//			modelComponent->setModel(assetManager.findAsset<Model>("Model_Makarov"));
//	//			modelComponent->setParent(anotherEntityTransform);
//	//		}
//	//		anotherEntityTransform->setPosition({ 0.0f, 2.0f, 0.0f });
//	//		anotherEntityTransform->setScale(glm::vec3(1.5f));
//	//		// anotherEntityTransform->setInheritScale(false);
//	//		// anotherEntityTransform->setInheritRotation(false);
//
//	//		if (Entity* anotherAnotherEntity = createEntity("ModelEntity2"))
//	//		{
//	//			auto transformComponent = anotherAnotherEntity->createComponent<TransformComponent>(anotherAnotherEntity);
//	//			transformComponent->setParent(anotherEntityTransform);
//	//			if (auto modelComponent = anotherAnotherEntity->createComponent<ModelComponent>(anotherAnotherEntity))
//	//			{
//	//				modelComponent->setModel(assetManager.findAsset<Model>("Model_Makarov"));
//	//				modelComponent->setParent(transformComponent);
//	//			}
//	//			transformComponent->setPosition({ 0.0f, 2.0f, 0.0f });
//	//			transformComponent->setScale(glm::vec3(1.5f));
//	//		}
//	//	}
//	//}
//
//	// Directional light
//	//if (Entity* directionalLightEntity = createEntityTemplate("DirectionalLight"))
//	//{
//	//	TransformComponent* lightTransform = directionalLightEntity->createComponent<TransformComponent>(directionalLightEntity);
//	//	
//	//	if (ModelComponent* lightModel = directionalLightEntity->createComponent<ModelComponent>(directionalLightEntity))
//	//	{
//	//		lightModel->setModel(apparatus->getAssetManager().findAsset<Model>("Model_DirectionalLight"));
//	//		lightModel->setParent(lightTransform);
//	//	}
//
//	//	if (auto directionalLightComponent = directionalLightEntity->createComponent<LightComponent>(directionalLightEntity))
//	//	{
//	//		directionalLightComponent->setColor({ 0.2f, 0.2f, 0.2f });
//	//	}
//	//}
//
//	//// Point light TEMPLATE
//	
//
//	// Point light
//	//if (Entity* pointLightEntity = createEntity("PointLight2"))
//	//{
//	//	auto lightTransform = createComponent<TransformComponent>(pointLightEntity);
//
//	//	if (auto lightModel = createComponent<ModelComponent>(pointLightEntity))
//	//	{
//	//		lightModel->setModel(apparatus->getAssetManager().findAsset<Model>("Model_PointLight"));
//	//		lightModel->setParent(lightTransform);
//	//	}
//
//	//	if (auto pointLightComponent = createComponent<LightComponent>(pointLightEntity))
//	//	{
//	//		pointLightComponent->setColor({ 1.0f, 1.0f, 1.0f });
//	//	}
//	//}
//
//	//// Spot light
//	//if (Entity* spotLightEntity = createEntity("SpotLight"))
//	//{
//	//	auto lightTransform = createComponent<TransformComponent>(spotLightEntity);
//
//	//	if (auto lightModel = createComponent<ModelComponent>(spotLightEntity))
//	//	{
//	//		lightModel->setModel(apparatus->getAssetManager().findAsset<Model>("Model_SpotLight"));
//	//		lightModel->setParent(lightTransform);
//	//	}
//
//	//	if (auto pointLightComponent = createComponent<LightComponent>(spotLightEntity))
//	//	{
//	//		pointLightComponent->setColor({ 1.0f, 0.0f, 1.0f });
//	//	}
//	//}
//
//	//for (size_t i = 0; i < 4; ++i)
//	//{
//	//	for (size_t j = 0; j < 4; ++j)
//	//	{
//	//		if (Entity* pointLightEntity = createEntity("PointLight"))
//	//		{
//	//			if (TransformComponent* lightTransform = createComponent<TransformComponent>(pointLightEntity))
//	//			{
//	//				lightTransform->setPosition({ i * 8, 6.0f, j * 8});
//	//				// lightTransform->offsetPosition({ -8.0f, 0.0f, 0.0f });
//	//				if (auto lightModel = createComponent<ModelComponent>(pointLightEntity))
//	//				{
//	//					lightModel->setModel(apparatus->getAssetManager().findAsset<Model>("Model_PointLight"));
//	//					lightModel->setParent(lightTransform);
//	//				}
//
//	//				LocalClient* localClient = apparatus->getPrimaryLocalClient();
//
//	//				if (auto pointLightComponent = createComponent<LightComponent>(pointLightEntity))
//	//				{
//	//					pointLightComponent->setColor({ 1.0f, 0.8f, 0.8f });
//	//				}
//	//			}
//	//		}
//	//	}
//	//}
//
//	// In Editor for all entities except the player add SelectableComponent
//	//for (std::unique_ptr<Entity>& entityPtr : getAllEntities())
//	//{
//	//	if (Entity* entity = entityPtr.get())
//	//	{
//	//		if (entity->getObjectName() != "Player")
//	//		{
//	//			entity->createComponent<SelectableComponent>(entity);
//	//		}
//	//	}
//	//}
//
//	//for (auto mapIter = entityTemplates.begin(); mapIter != entityTemplates.end(); ++mapIter)
//	//{
//	//	if (Entity* entity = mapIter->second.get())
//	//	{
//	//		if (entity->getObjectName() != "Player")
//	//		{
//	//			entity->createComponent<SelectableComponent>(entity);
//	//		}
//	//	}
//	//}
//
//	setupGizmoEntity();
//}
//
//void EditorLocalServer::setupGizmoEntity()
//{
//	
//}
