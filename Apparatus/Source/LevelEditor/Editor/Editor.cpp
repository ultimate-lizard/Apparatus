#include "Editor.h"

#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/LightComponent.h>
#include <Apparatus/Core/EntityRegistry.h>
#include <Apparatus/Core/AssetManager.h>
// #include <Apparatus/Apparatus.h>
#include "../Server/EditorLocalServer.h"
#include "../Client/EditorLocalClient.h"
#include "../Components/GizmoComponent.h"

Editor::Editor(const std::string& gameTitle) :
	Apparatus(gameTitle)
{
}

void Editor::init()
{
	Apparatus::init();

	createServer<EditorLocalServer>();

	Renderer* renderer = findEngineSystem<Renderer>();
	SpriteRenderer* spriteRenderer = findEngineSystem<SpriteRenderer>();

	createClient<EditorLocalClient>(renderer, spriteRenderer);
}

void Editor::_createEntityTemplates()
{
	Apparatus::_createEntityTemplates();

	EntityRegistry* entityRegistry = findEngineSystem<EntityRegistry>();
	AssetManager* assetManager = findEngineSystem<AssetManager>();
	if (!entityRegistry || !assetManager)
	{
		return;
	}

	if (Entity* pointLightEntity = entityRegistry->createEntityTemplate("PointLight"))
	{
		TransformComponent* lightTransform = entityRegistry->createComponent<TransformComponent>(pointLightEntity);

		if (ModelComponent* lightModel = entityRegistry->createComponent<ModelComponent>(pointLightEntity))
		{
			lightModel->setModel(assetManager->findAsset<Model>("Model_PointLight"));
			lightModel->setParent(lightTransform);
		}

		if (LightComponent* pointLightComponent = entityRegistry->createComponent<LightComponent>(pointLightEntity, LightType::PointLight))
		{
			pointLightComponent->setParent(lightTransform);
		}
	}

	if (Entity* spotLightEntity = entityRegistry->createEntityTemplate("SpotLight"))
	{
		TransformComponent* lightTransform = entityRegistry->createComponent<TransformComponent>(spotLightEntity);

		if (ModelComponent* lightModel = entityRegistry->createComponent<ModelComponent>(spotLightEntity))
		{
			lightModel->setModel(assetManager->findAsset<Model>("Model_SpotLight"));
			lightModel->setParent(lightTransform);
		}

		if (LightComponent* spotLightComponent = entityRegistry->createComponent<LightComponent>(spotLightEntity, LightType::SpotLight))
		{
			spotLightComponent->setParent(lightTransform);
		}
	}

	if (Entity* directionalLightEntity = entityRegistry->createEntityTemplate("DirectionalLight"))
	{
		TransformComponent* lightTransform = entityRegistry->createComponent<TransformComponent>(directionalLightEntity);

		if (ModelComponent* lightModel = entityRegistry->createComponent<ModelComponent>(directionalLightEntity))
		{
			lightModel->setModel(assetManager->findAsset<Model>("Model_DirectionalLight"));
			lightModel->setParent(lightTransform);
		}

		if (LightComponent* directionalLightComponent = entityRegistry->createComponent<LightComponent>(directionalLightEntity, LightType::DirectionalLight))
		{
			directionalLightComponent->setParent(lightTransform);
		}
	}

	if (Entity* selectionProxy = entityRegistry->createEntityTemplate("SelectionProxy"))
	{
		TransformComponent* proxyTransformComponent = entityRegistry->createComponent<TransformComponent>(selectionProxy);
	}

	if (Entity* gizmo = entityRegistry->createEntityTemplate("Gizmo"))
	{
		if (auto gizmoTransform = entityRegistry->createComponent<TransformComponent>(gizmo))
		{
			gizmoTransform->setInheritRotation(false);
			gizmoTransform->setInheritScale(false);

			entityRegistry->createComponent<GizmoComponent>(gizmo);

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "GizmoX"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_GizmoX"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "GizmoY"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_GizmoY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "GizmoZ"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_GizmoZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "GizmoXY"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_GizmoXY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "GizmoXZ"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_GizmoXZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "GizmoZY"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_GizmoYZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto rollModelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "Roll"))
			{
				rollModelComponent->setModel(assetManager->findAsset<Model>("Model_Roll"));
				rollModelComponent->setParent(gizmoTransform);
				rollModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

				if (auto yawModelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "Yaw"))
				{
					yawModelComponent->setModel(assetManager->findAsset<Model>("Model_Yaw"));
					yawModelComponent->setParent(rollModelComponent);
					yawModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

					if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "Pitch"))
					{
						modelComponent->setModel(assetManager->findAsset<Model>("Model_Pitch"));
						modelComponent->setParent(yawModelComponent);
						modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
					}
				}
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "ScaleUp"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_ScaleUp"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "ScaleDown"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_ScaleDown"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "ScaleLeft"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_ScaleLeft"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "ScaleRight"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_ScaleRight"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "ScaleFront"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_ScaleFront"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "ScaleBack"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_ScaleBack"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(gizmo, "ScaleAll"))
			{
				modelComponent->setModel(assetManager->findAsset<Model>("Model_ScaleAll"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}
		}
	}
}
