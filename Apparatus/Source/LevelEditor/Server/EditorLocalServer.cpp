#include "EditorLocalServer.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Rendering/Renderer.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/DirectionalLightComponent.h>
#include <Apparatus/Components/PointLightComponent.h>
#include <Apparatus/Components/SpotLightComponent.h>

#include "../Components/SelectableComponent.h"
#include "../Components/GizmoComponent.h"

EditorLocalServer::EditorLocalServer(Apparatus* apparatus) :
	LocalServer(apparatus)
{
	assignDefaultObjectName();
}

void EditorLocalServer::update(float dt)
{
	LocalServer::update(dt);

	// Render visual boxes of selected entities
	for (std::unique_ptr<Entity>& entityPtr : getAllEntities())
	{
		if (Entity* entity = entityPtr.get())
		{
			if (auto selectable = entity->findComponent<SelectableComponent>())
			{
				if (selectable->isSelected() && selectable->isBoxVisible())
				{
					selectable->updateVisualBoundingBoxPosition();
					Box box = selectable->getVisualBoundingBox();
					drawDebugBox(box, { 1.0f, 1.0f, 0.0f, 1.0f }, 1.0f);
				}
			}
		}
	}
}

void EditorLocalServer::assignDefaultObjectName()
{
	setObjectName("EditorLocalServer");
}

void EditorLocalServer::setupEntities()
{
	LocalServer::setupEntities();

	if (!apparatus)
	{
		return;
	}

	AssetManager& assetManager = apparatus->getResourceManager();

	if (Entity* modelEntity = createEntity("ModelEntity"))
	{
		auto modelTransform = createComponent<TransformComponent>(modelEntity);
		if (auto modelComponent = createComponent<ModelComponent>(modelEntity))
		{
			modelComponent->setModel(assetManager.findAsset<Model>("Model_Makarov"));
			modelComponent->setParent(modelTransform);
		}
		modelTransform->setPosition({ 3.0f, 0.0f, 14.0f });
		modelTransform->setScale(glm::vec3(1.5f));
		
		if (Entity* anotherEntity = createEntity("ModelEntity2"))
		{
			auto anotherEntityTransform = createComponent<TransformComponent>(anotherEntity);
			anotherEntityTransform->setParent(modelTransform);
			if (auto modelComponent = createComponent<ModelComponent>(anotherEntity))
			{
				modelComponent->setModel(assetManager.findAsset<Model>("Model_Makarov"));
				modelComponent->setParent(anotherEntityTransform);
			}
			anotherEntityTransform->setPosition({ 0.0f, 2.0f, 0.0f });
			anotherEntityTransform->setScale(glm::vec3(1.5f));
			// anotherEntityTransform->setInheritScale(false);
			// anotherEntityTransform->setInheritRotation(false);

			if (Entity* anotherAnotherEntity = createEntity("ModelEntity2"))
			{
				auto transformComponent = createComponent<TransformComponent>(anotherAnotherEntity);
				transformComponent->setParent(anotherEntityTransform);
				if (auto modelComponent = createComponent<ModelComponent>(anotherAnotherEntity))
				{
					modelComponent->setModel(assetManager.findAsset<Model>("Model_Makarov"));
					modelComponent->setParent(transformComponent);
				}
				transformComponent->setPosition({ 0.0f, 2.0f, 0.0f });
				transformComponent->setScale(glm::vec3(1.5f));
			}
		}
	}

	//// Directional light
	//if (Entity* directionalLightEntity = createEntity("DirectionalLight"))
	//{
	//	auto lightTransform = createComponent<TransformComponent>(directionalLightEntity);
	//	
	//	if (auto lightModel = createComponent<ModelComponent>(directionalLightEntity))
	//	{
	//		lightModel->setModel(apparatus->getResourceManager().findAsset<Model>("Model_DirectionalLight"));
	//		lightModel->setParent(lightTransform);
	//	}

	//	if (auto directionalLightComponent = createComponent<DirectionalLightComponent>(directionalLightEntity))
	//	{
	//		directionalLightComponent->setDiffuseColor({ 0.2f, 0.2f, 0.4f });
	//		directionalLightComponent->setAmbientColor({ 0.0f, 0.0f, 0.1f });
	//		directionalLightComponent->setSpecularColor({ 0.0f, 0.0f, 0.1f });
	//	}
	//}

	//// Point light
	//if (Entity* pointLightEntity = createEntity("PointLight"))
	//{
	//	auto lightTransform = createComponent<TransformComponent>(pointLightEntity);

	//	if (auto lightModel = createComponent<ModelComponent>(pointLightEntity))
	//	{
	//		lightModel->setModel(apparatus->getResourceManager().findAsset<Model>("Model_PointLight"));
	//		lightModel->setParent(lightTransform);
	//	}

	//	if (auto pointLightComponent = createComponent<PointLightComponent>(pointLightEntity))
	//	{
	//		pointLightComponent->setAmbientColor({ 0.5f, 0.3f, 0.0f });
	//		pointLightComponent->setDiffuseColor({ 1.0f, 0.7f, 0.0f });
	//		pointLightComponent->setSpecularColor({ 1.0f, 0.7f, 0.0f });
	//	}
	//}

	//// Point light
	if (Entity* pointLightEntity = createEntity("PointLight2"))
	{
		auto lightTransform = createComponent<TransformComponent>(pointLightEntity);

		if (auto lightModel = createComponent<ModelComponent>(pointLightEntity))
		{
			lightModel->setModel(apparatus->getResourceManager().findAsset<Model>("Model_PointLight"));
			lightModel->setParent(lightTransform);
		}

		if (auto pointLightComponent = createComponent<PointLightComponent>(pointLightEntity))
		{
			pointLightComponent->setAmbientColor({ 0.5f, 0.5f, 0.5f });
			pointLightComponent->setDiffuseColor({ 1.0f, 0.0f, 1.0f });
			pointLightComponent->setSpecularColor({ 1.0f, 0.0f, 1.0f });
		}
	}

	// Spot light
	if (Entity* spotLightEntity = createEntity("SpotLight"))
	{
		auto lightTransform = createComponent<TransformComponent>(spotLightEntity);

		if (auto lightModel = createComponent<ModelComponent>(spotLightEntity))
		{
			lightModel->setModel(apparatus->getResourceManager().findAsset<Model>("Model_SpotLight"));
			lightModel->setParent(lightTransform);
		}

		if (auto pointLightComponent = createComponent<SpotLightComponent>(spotLightEntity))
		{
			pointLightComponent->setAmbientColor({ 0.5f, 0.5f, 0.5f });
			pointLightComponent->setDiffuseColor({ 1.0f, 1.0f, 0.8f });
			pointLightComponent->setSpecularColor({ 1.0f, 1.0f, 0.8f });
		}
	}

	// In Editor for all entities except the player add SelectableComponent
	for (std::unique_ptr<Entity>& entityPtr : getAllEntities())
	{
		if (Entity* entity = entityPtr.get())
		{
			if (entity->getObjectName() != "Player")
			{
				createComponent<SelectableComponent>(entity);
			}
		}
	}

	setupGizmoEntity();
}

void EditorLocalServer::setupGizmoEntity()
{
	if (Entity* gizmo = createEntity("Gizmo"))
	{
		if (auto gizmoTransform = createComponent<TransformComponent>(gizmo))
		{
			gizmoTransform->setInheritRotation(false);
			gizmoTransform->setInheritScale(false);

			createComponent<GizmoComponent>(gizmo, "GizmoX");

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoX"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoX"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoY"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoXY"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoXY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoXZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoXZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoZY"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoYZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto rollModelComponent = createComponent<ModelComponent>(gizmo, "Roll"))
			{
				rollModelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_Roll"));
				rollModelComponent->setParent(gizmoTransform);
				rollModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

				if (auto yawModelComponent = createComponent<ModelComponent>(gizmo, "Yaw"))
				{
					yawModelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_Yaw"));
					yawModelComponent->setParent(rollModelComponent);
					yawModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

					if (auto modelComponent = createComponent<ModelComponent>(gizmo, "Pitch"))
					{
						modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_Pitch"));
						modelComponent->setParent(yawModelComponent);
						modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
					}
				}
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "ScaleUp"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_ScaleUp"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "ScaleDown"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_ScaleDown"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "ScaleLeft"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_ScaleLeft"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "ScaleRight"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_ScaleRight"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "ScaleFront"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_ScaleFront"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "ScaleBack"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_ScaleBack"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "ScaleAll"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_ScaleAll"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}
		}
	}
}
