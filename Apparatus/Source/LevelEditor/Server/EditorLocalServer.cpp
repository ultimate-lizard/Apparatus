#include "EditorLocalServer.h"

#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Apparatus.h>

#include "../Components/SelectableComponent.h"

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
		auto transformComponent = createComponent<TransformComponent>(modelEntity);
		if (auto modelComponent = createComponent<ModelComponent>(modelEntity))
		{
			modelComponent->setModel(assetManager.findAsset<Model>("Model_Makarov"));
			modelComponent->setParent(transformComponent);
		}
		transformComponent->setPosition({ 3.0f, 0.0f, 14.0f });
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
		if (auto transform = createComponent<TransformComponent>(gizmo))
		{
			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoX"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoX"));
				modelComponent->setParent(transform);
				modelComponent->setInheritRotation(false);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoY"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoY"));
				modelComponent->setParent(transform);
				modelComponent->setInheritRotation(false);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoZ"));
				modelComponent->setParent(transform);
				modelComponent->setInheritRotation(false);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoXY"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoXY"));
				modelComponent->setParent(transform);
				modelComponent->setInheritRotation(false);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoXZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoXZ"));
				modelComponent->setParent(transform);
				modelComponent->setInheritRotation(false);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoYZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoYZ"));
				modelComponent->setParent(transform);
				modelComponent->setInheritRotation(false);
			}

			if (auto rollModelComponent = createComponent<ModelComponent>(gizmo, "Roll"))
			{
				rollModelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_Roll"));
				rollModelComponent->setParent(transform);
				rollModelComponent->setInheritRotation(false);

				if (auto yawModelComponent = createComponent<ModelComponent>(gizmo, "Yaw"))
				{
					yawModelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_Yaw"));
					yawModelComponent->setParent(rollModelComponent);

					if (auto modelComponent = createComponent<ModelComponent>(gizmo, "Pitch"))
					{
						modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_Pitch"));
						modelComponent->setParent(yawModelComponent);
					}
				}
			}
		}
	}
}
