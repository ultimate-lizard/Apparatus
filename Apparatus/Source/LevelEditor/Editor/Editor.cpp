#include "Editor.h"

#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/LightComponent/PointLightComponent.h>

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
	createClient<EditorLocalClient>(getRenderer());
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

		if (PointLightComponent* pointLightComponent = Apparatus::getEntityRegistry().createComponent<PointLightComponent>(pointLightEntity))
		{
			pointLightComponent->setParent(lightTransform);
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
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_GizmoX"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoY"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_GizmoY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoZ"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_GizmoZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoXY"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_GizmoXY"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoXZ"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_GizmoXZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "GizmoZY"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_GizmoYZ"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto rollModelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "Roll"))
			{
				rollModelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_Roll"));
				rollModelComponent->setParent(gizmoTransform);
				rollModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

				if (auto yawModelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "Yaw"))
				{
					yawModelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_Yaw"));
					yawModelComponent->setParent(rollModelComponent);
					yawModelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());

					if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "Pitch"))
					{
						modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_Pitch"));
						modelComponent->setParent(yawModelComponent);
						modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
					}
				}
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleUp"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleUp"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleDown"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleDown"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleLeft"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleLeft"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleRight"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleRight"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleFront"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleFront"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleBack"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleBack"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}

			if (auto modelComponent = Apparatus::getEntityRegistry().createComponent<ModelComponent>(gizmo, "ScaleAll"))
			{
				modelComponent->setModel(Apparatus::getAssetManager().findAsset<Model>("Model_Mesh_ScaleAll"));
				modelComponent->setParent(gizmoTransform);
				modelComponent->setDepthBufferLayer(Renderer::getMaxDepthBufferLayer());
			}
		}
	}
}
