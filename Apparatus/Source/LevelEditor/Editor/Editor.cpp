#include "Editor.h"

#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/LightComponent.h>
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
	createClient<EditorLocalClient>(getRenderer(), getSpriteRenderer());
}

void Editor::_createEntityTemplates()
{
	Apparatus::_createEntityTemplates();

	if (Entity* pointLightEntity = Apparatus::getEntityRegistry().createEntityTemplate("PointLight"))
	{
		TransformComponent* lightTransform = Apparatus::getEntityRegistry().createComponent<TransformComponent>(pointLightEntity);

		if (ModelComponent* lightModel = Apparatus::getEntityRegistry().createComponent<ModelComponent>(pointLightEntity))
		{
			lightModel->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_PointLight"));
			lightModel->setParent(lightTransform);
		}

		if (LightComponent* pointLightComponent = Apparatus::getEntityRegistry().createComponent<LightComponent>(pointLightEntity, LightType::PointLight))
		{
			pointLightComponent->setParent(lightTransform);
		}
	}

	if (Entity* spotLightEntity = Apparatus::getEntityRegistry().createEntityTemplate("SpotLight"))
	{
		TransformComponent* lightTransform = Apparatus::getEntityRegistry().createComponent<TransformComponent>(spotLightEntity);

		if (ModelComponent* lightModel = Apparatus::getEntityRegistry().createComponent<ModelComponent>(spotLightEntity))
		{
			lightModel->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_SpotLight"));
			lightModel->setParent(lightTransform);
		}

		if (LightComponent* spotLightComponent = Apparatus::getEntityRegistry().createComponent<LightComponent>(spotLightEntity, LightType::SpotLight))
		{
			spotLightComponent->setParent(lightTransform);
		}
	}

	if (Entity* directionalLightEntity = Apparatus::getEntityRegistry().createEntityTemplate("DirectionalLight"))
	{
		TransformComponent* lightTransform = Apparatus::getEntityRegistry().createComponent<TransformComponent>(directionalLightEntity);

		if (ModelComponent* lightModel = Apparatus::getEntityRegistry().createComponent<ModelComponent>(directionalLightEntity))
		{
			lightModel->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_DirectionalLight"));
			lightModel->setParent(lightTransform);
		}

		if (LightComponent* directionalLightComponent = Apparatus::getEntityRegistry().createComponent<LightComponent>(directionalLightEntity, LightType::DirectionalLight))
		{
			directionalLightComponent->setParent(lightTransform);
		}
	}

	if (Entity* selectionProxy = Apparatus::getEntityRegistry().createEntityTemplate("SelectionProxy"))
	{
		TransformComponent* proxyTransformComponent = Apparatus::getEntityRegistry().createComponent<TransformComponent>(selectionProxy);
	}

	if (Entity* gizmo = Apparatus::getEntityRegistry().createEntityTemplate("Gizmo"))
	{
		if (auto gizmoTransform = Apparatus::getEntityRegistry().createComponent<TransformComponent>(gizmo))
		{
			gizmoTransform->setInheritRotation(false);
			gizmoTransform->setInheritScale(false);

			Apparatus::getEntityRegistry().createComponent<GizmoComponent>(gizmo);

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoX"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_GizmoX"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoY"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_GizmoY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoZ"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_GizmoZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoXY"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_GizmoXY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoXZ"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_GizmoXZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoZY"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_GizmoYZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto rollModelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "Roll"))
			{
				rollModelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Roll"));
				rollModelComponent->setParent(gizmoTransform);
				rollModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

				if (auto yawModelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "Yaw"))
				{
					yawModelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Yaw"));
					yawModelComponent->setParent(rollModelComponent);
					yawModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

					if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "Pitch"))
					{
						modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Pitch"));
						modelComponent->setParent(yawModelComponent);
						modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
					}
				}
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleUp"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_ScaleUp"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleDown"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_ScaleDown"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleLeft"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_ScaleLeft"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleRight"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_ScaleRight"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleFront"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_ScaleFront"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleBack"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_ScaleBack"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleAll"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_ScaleAll"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}
		}
	}
}
