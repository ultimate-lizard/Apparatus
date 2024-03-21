#include "LocalServer.h"

#include <glm/gtc/quaternion.hpp>

#include "../Core/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/DebugMovementComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/SelectableComponent.h"
#include "../Apparatus.h"
#include "../Rendering/Material.h"
#include "../Common/BoundingBox.h"

DebugPrimitives LocalServer::debugPrimitives;

LocalServer::LocalServer(Apparatus* apparatus) :
	Server(apparatus)
{
}

void LocalServer::init()
{
	createEntities();

	for (std::unique_ptr<Component>& component : components)
	{
		if (component)
		{
			component->init();
		}
	}
}

void LocalServer::update(float dt)
{
	for (auto& pointsIter : debugPrimitives.debugPoints)
	{
		updateDebugPrimitiveVector<Point>(pointsIter.second, dt);
	}

	for (auto& linesIter : debugPrimitives.debugLines)
	{
		updateDebugPrimitiveVector<Line>(linesIter.second, dt);
	}

	for (auto& boxesIter : debugPrimitives.debugBoxes)
	{
		updateDebugPrimitiveVector<Box>(boxesIter.second, dt);
	}

	for (std::unique_ptr<Component>& component : components)
	{
		if (component)
		{
			component->update(dt);
		}
	}

	for (std::unique_ptr<Component>& component : components)
	{
		if (auto selectable = dynamic_cast<SelectableComponent*>(component.get()))
		{
			if (selectable->isSelected())
			{
				const Box& box = selectable->getBoundingBox();
				drawRectangle({ box.size, box.position }, { 1.0f, 1.0f, 0.0f, 1.0f }, 1.0f);
			}
		}
	}
}

std::vector<std::unique_ptr<Entity>>& LocalServer::getAllEntities()
{
	return entities;
}

Entity* LocalServer::createEntity(const std::string& id)
{
	entities.push_back(std::make_unique<Entity>(id));
	return entities.back().get();
}

void LocalServer::drawPoint(const glm::vec3& position, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	Point debugPoint;
	debugPoint.position = position;

	DebugPrimitiveData debugPrimitiveData;
	debugPrimitiveData.color = color;
	debugPrimitiveData.lifeTime = persistent ? -1.0f : lifetime;
	debugPrimitiveData.vertexCount = 1;

	debugPrimitives.debugPoints[drawSize].push_back({ debugPoint, debugPrimitiveData });
}

void LocalServer::drawLine(const Line& line, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	DebugPrimitiveData debugPrimitiveData;
	debugPrimitiveData.color = color;
	debugPrimitiveData.lifeTime = persistent ? -1.0f : lifetime;
	debugPrimitiveData.vertexCount = 2;

	debugPrimitives.debugLines[drawSize].push_back({ line, debugPrimitiveData });
}

void LocalServer::drawRectangle(const Box& box, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	DebugPrimitiveData debugPrimitiveData;
	debugPrimitiveData.color = color;
	debugPrimitiveData.lifeTime = persistent ? -1.0f : lifetime;
	debugPrimitiveData.vertexCount = 8;

	debugPrimitives.debugBoxes[drawSize].push_back({ box, debugPrimitiveData });
}

const DebugPrimitives& LocalServer::getDebugPrimitives() const
{
	return debugPrimitives;
}

std::vector<RayTraceResult> LocalServer::traceRay(const glm::vec3& direction, const glm::vec3& position, float length)
{
	//drawLine({ position, position + (direction * length) }, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.0f, false, 3.0f);

	std::vector<std::pair<Box, Entity*>> boxes;
	for (std::unique_ptr<Entity>& entity : getAllEntities())
	{
		if (entity.get())
		{
			if (auto selectableComponent = entity->findComponent<SelectableComponent>())
			{
				boxes.push_back({ selectableComponent->getBoundingBox(), entity.get()});
			}
		}
	}

	std::vector<RayTraceResult> resultVector;

	for (const auto& pair : boxes)
	{
		auto castResult = rayVsAABB(position, direction * length, pair.first);
		if (castResult.first != glm::vec3(0.0f) && castResult.second != glm::vec3(0.0f))
		{
			RayTraceResult result;
			result.near = castResult.first;
			result.far = castResult.second;
			result.entity = pair.second;

			resultVector.push_back(result);
		}
	}

	return resultVector;
}

void LocalServer::createEntities()
{
	// TODO: assert apparatus
	ResourceManager& resourceManager = apparatus->getResourceManager();

	if (Entity* player = createEntity("Player"))
	{
		if (auto transformComponent = createComponent<TransformComponent>(player, "Transform"))
		{
			if (auto cameraComponent = createComponent<CameraComponent>(player, "Camera"))
			{
				Camera& camera = cameraComponent->getCamera();
				camera.setParent(transformComponent);
			}

			transformComponent->setPosition({ 10.0f, 10.0f, 10.0f });
		}

		auto movementComponent = createComponent<MovementComponent>(player, "Movement");
	}

	if (Entity* sceneEntity = createEntity("SceneEntity"))
	{
		auto transformComponent = createComponent<TransformComponent>(sceneEntity, "Transform");
		if (auto modelComponent = createComponent<ModelComponent>(sceneEntity, "Model"))
		{
			modelComponent->setModel(resourceManager.findResource<Model>("Model_Scene"));
			modelComponent->setParent(transformComponent);
		}
	}

	if (Entity* cube = createEntity("Cube"))
	{
		if (auto transformComponent = createComponent<TransformComponent>(cube, "Transform"))
		{
			//transformComponent->setPosition({ 0.0f, 0.0f, 10.0f });
			//transformComponent->rotate(45.0f, Euler::Yaw);

			if (auto modelComponent = createComponent<ModelComponent>(cube, "Model"))
			{
				modelComponent->setModel(resourceManager.findResource<Model>("Model_Cube"));
				modelComponent->setParent(transformComponent);
			}
		}

		auto selectable = createComponent<SelectableComponent>(cube, "Selectable");
	}

	if (Entity* cube2 = createEntity("Cube2"))
	{
		if (auto transformComponent = createComponent<TransformComponent>(cube2, "Transform"))
		{
			transformComponent->setPosition({ 0.0f, 2.0f, 0.0f });
			//transformComponent->rotate(45.0f, Euler::Yaw);

			if (auto modelComponent = createComponent<ModelComponent>(cube2, "Model"))
			{
				modelComponent->setModel(resourceManager.findResource<Model>("Model_Cube"));
				modelComponent->setParent(transformComponent);
			}
		}

		auto selectable = createComponent<SelectableComponent>(cube2, "Selectable");
	}

	if (Entity* cube = findEntity("Cube"))
	{
		if (Entity* cube2 = findEntity("Cube2"))
		{
			if (auto cube2TransformComponent = cube2->findComponent<TransformComponent>())
			{
				if (auto cubeTransformComponent = cube->findComponent<TransformComponent>())
				{
					cube2TransformComponent->setParent(cubeTransformComponent);

					if (Entity* player = findEntity("Player"))
					{
						if (auto playerTransform = player->findComponent<TransformComponent>())
						{
							playerTransform->setParent(cubeTransformComponent);
						}
					}
				}
			}
		}
	}
}

Entity* LocalServer::findEntity(const std::string& id)
{
	auto iter = std::find_if(entities.begin(), entities.end(), [id](std::unique_ptr<Entity>& entityPtr) {
		if (Entity* entity = entityPtr.get())
		{
			return entity->getId() == id;
		}

		return false;
	});

	if (iter != entities.end())
	{
		return iter->get();
	}

	return nullptr;
}
