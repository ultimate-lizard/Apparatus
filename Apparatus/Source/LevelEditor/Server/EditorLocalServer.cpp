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
				if (selectable->isSelected())
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

	if (Entity* gizmo = createEntity("Gizmo"))
	{
		if (auto transform = createComponent<TransformComponent>(gizmo))
		{
			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoX"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoX"));
				modelComponent->setParent(transform);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoY"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoY"));
				modelComponent->setParent(transform);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoZ"));
				modelComponent->setParent(transform);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoXY"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoXY"));
				modelComponent->setParent(transform);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoXZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoXZ"));
				modelComponent->setParent(transform);
			}

			if (auto modelComponent = createComponent<ModelComponent>(gizmo, "GizmoYZ"))
			{
				modelComponent->setModel(apparatus->getResourceManager().findAsset<Model>("Model_Mesh_GizmoYZ"));
				modelComponent->setParent(transform);
			}
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
}
