#include "EditorLocalServer.h"

#include <Apparatus/Level.h>
#include <Apparatus/Util/BoundingBox.h>
#include <Apparatus/Rendering/Renderer.h>
#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/LightComponent/DirectionalLightComponent.h>
#include <Apparatus/Components/LightComponent/PointLightComponent.h>
#include <Apparatus/Components/LightComponent/SpotLightComponent.h>

#include "../Components/SelectableComponent.h"
#include "../Components/GizmoComponent.h"
#include "../Editor/Editor.h"

EditorLocalServer::EditorLocalServer() :
	LocalServer(),
	selection(nullptr),
	gizmo(nullptr),
	cachedSelectionPosition(0.0f),
	selectionBoxVisible(false),
	snapToGrid(false)
{
}

void EditorLocalServer::connect(const ConnectionInfo& info)
{
	LocalServer::connect(info);


}

void EditorLocalServer::update(float dt)
{
	LocalServer::update(dt);

	if (selection)
	{
		if (auto transform = selection->findComponent<TransformComponent>())
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
	}
}

void EditorLocalServer::start()
{
	LocalServer::start();

	if (level)
	{
		Entity* player = level->spawnEntity("Player");

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

		/*for (size_t i = 0; i < 11; ++i)
		{
			for (size_t j = 0; j < 11; ++j)
			{
				if (Entity* pointLight = level->spawnEntity("PointLight"))
				{
					if (TransformComponent* lightTransform = pointLight->findComponent<TransformComponent>())
					{
						lightTransform->setPosition({ i * 4.0f, 5.0f, j * 4.0f });
					}
				}
			}
		}*/

		if (Entity* pointLight = level->spawnEntity("PointLight"))
		{
			if (PointLightComponent* pointLightComponent = pointLight->findComponent<PointLightComponent>())
			{
				pointLightComponent->setColor({ 1.0f, 0.8f, 0.4f });
			}
		}
	}
}

void EditorLocalServer::selectEntity(Entity* entity)
{
	indicateSelection(selection, false);

	this->selection = entity;

	if (entity)
	{
		indicateSelection(entity, true);
		regenerateSelectionBoundingBox();
	}

	if (gizmo)
	{
		gizmo->attach(entity);
		gizmo->setVisibility(entity != nullptr);
	}
}

Entity* EditorLocalServer::getSelectedEntity()
{
	return selection;
}

void EditorLocalServer::duplicateSelection()
{
	if (Entity* newSelection = Apparatus::getEntityRegistry().clone(selection))
	{
		newSelection->init();
		newSelection->onSpawn();

		if (level)
		{
			level->addEntity(newSelection);
		}

		selectEntity(newSelection);
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
	if (!selection)
	{
		return;
	}

	auto transformComponent = selection->findComponent<TransformComponent>();
	if (!transformComponent)
	{
		return;
	}

	cachedSelectionPosition = transformComponent->getWorldPosition();

	std::vector<ModelComponent*> modelComponents = selection->getAllComponentsOfClass<ModelComponent>();
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
	}
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
//	//	if (auto directionalLightComponent = directionalLightEntity->createComponent<DirectionalLightComponent>(directionalLightEntity))
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
//	//	if (auto pointLightComponent = createComponent<PointLightComponent>(pointLightEntity))
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
//	//	if (auto pointLightComponent = createComponent<SpotLightComponent>(spotLightEntity))
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
//	//				if (auto pointLightComponent = createComponent<PointLightComponent>(pointLightEntity))
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
