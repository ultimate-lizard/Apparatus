#include "LocalServer.h"

#include <glm/gtc/quaternion.hpp>

#include "../Core/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/DebugMovementComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Apparatus.h"
#include "../Rendering/Material.h"
#include "../Util/CollisionDetection.h"

DebugPrimitives LocalServer::debugPrimitives;

LocalServer::LocalServer(Apparatus* apparatus) :
	Server(apparatus)
{
	assignDefaultObjectName();
}

void LocalServer::init()
{
	setupEntities();

	for (std::unique_ptr<Component>& component : components)
	{
		if (component)
		{
			component->init();
		}
	}

	for (std::unique_ptr<Entity>& entity : entities)
	{
		if (entity)
		{
			entity->init();
		}
	}

	for (std::unique_ptr<Component>& component : components)
	{
		if (component)
		{
			component->start();
		}
	}

	for (std::unique_ptr<Entity>& entity : entities)
	{
		if (entity)
		{
			entity->start();
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

	for (std::unique_ptr<Entity>& entity : entities)
	{
		if (entity)
		{
			entity->update(dt);
		}
	}
}

void LocalServer::assignDefaultObjectName()
{
	setObjectName("LocalServer");
}

void LocalServer::setupEntities()
{
	assert(apparatus);
	AssetManager& resourceManager = apparatus->getResourceManager();

	if (Entity* player = createEntity("Player"))
	{
		if (auto transformComponent = createComponent<TransformComponent>(player))
		{
			if (auto cameraComponent = createComponent<CameraComponent>(player))
			{
				Camera& camera = cameraComponent->getCamera();
				camera.setParent(transformComponent);
			}
		}

		auto movementComponent = createComponent<MovementComponent>(player);
	}

	if (Entity* defaultSceneEntity = createEntity("DefaultScene"))
	{
		auto transformComponent = createComponent<TransformComponent>(defaultSceneEntity);
		if (auto modelComponent = createComponent<ModelComponent>(defaultSceneEntity))
		{
			modelComponent->setModel(resourceManager.findAsset<Model>("Model_Scene"));
			modelComponent->setParent(transformComponent);
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

void LocalServer::drawDebugPoint(const glm::vec3& position, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	Point debugPoint;
	debugPoint.position = position;

	DebugPrimitiveData debugPrimitiveData;
	debugPrimitiveData.color = color;
	debugPrimitiveData.lifeTime = persistent ? -1.0f : lifetime;
	debugPrimitiveData.vertexCount = 1;

	debugPrimitives.debugPoints[drawSize].push_back({ debugPoint, debugPrimitiveData });
}

void LocalServer::drawDebugLine(const Line& line, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	DebugPrimitiveData debugPrimitiveData;
	debugPrimitiveData.color = color;
	debugPrimitiveData.lifeTime = persistent ? -1.0f : lifetime;
	debugPrimitiveData.vertexCount = 2;

	debugPrimitives.debugLines[drawSize].push_back({ line, debugPrimitiveData });
}

void LocalServer::drawDebugBox(const Box& box, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
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

std::vector<RayTraceResult> LocalServer::traceRay(const glm::vec3& origin, const glm::vec3& direction, DetectionType detectionType)
{
	if (detectionType != DetectionType::Visibility)
	{
		return {};
	}

	std::vector<RayTraceResult> result;
	for (std::unique_ptr<Entity>& entity : getAllEntities())
	{
		if (!entity)
		{
			continue;
		}

		for (ModelComponent* modelComponent : entity->getAllComponentsOfClass<ModelComponent>())
		{
			if (!modelComponent)
			{
				continue;
			}

			if (!modelComponent->isVisible())
			{
				continue;
			}

			if (Model* model = modelComponent->getModel())
			{
				for (Mesh* mesh : model->getMeshes())
				{
					auto intersections = rayVsMesh(origin, direction, mesh, modelComponent->getTransform());
					for (const auto& intersectionPair : intersections)
					{
						RayTraceResult traceResult;
						traceResult.near = intersectionPair.first;
						traceResult.far = intersectionPair.second;
						traceResult.entity = entity.get();
						traceResult.modelComponent = modelComponent;

						result.push_back(std::move(traceResult));
					}
				}
			}
		}
	}

	// Sort the results by distance to the origin
	std::sort(result.begin(), result.end(), [&origin](RayTraceResult& a, RayTraceResult& b) {
		return glm::distance(a.near, origin) < glm::distance(b.near, origin);
	});

	return result;
}

glm::vec3 LocalServer::getCursorToWorldRay(const glm::mat4& view, const glm::mat4& projection)
{
	glm::ivec2 mousePos = apparatus->getMouseCursorPosition();
	glm::vec2 windowSize = apparatus->getWindowSize();

	float normalX = (2.0f * mousePos.x) / windowSize.x - 1.0f;
	float normalY = 1.0f - (2.0f * mousePos.y) / windowSize.y;

	glm::vec4 rayClip(normalX, normalY, -1.0f, 1.0f);

	glm::vec4 rayEye = glm::inverse(projection) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	return glm::normalize(glm::inverse(view) * rayEye);
}

Entity* LocalServer::findEntity(const std::string& id)
{
	auto iter = std::find_if(entities.begin(), entities.end(), [id](std::unique_ptr<Entity>& entityPtr) {
		if (Entity* entity = entityPtr.get())
		{
			return entity->getObjectName() == id;
		}

		return false;
	});

	if (iter != entities.end())
	{
		return iter->get();
	}

	return nullptr;
}
