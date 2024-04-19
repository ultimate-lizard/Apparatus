#include "Level.h"

#include "Server/Entity.h"
#include "Server/LocalServer.h"
#include "Rendering/Renderer.h"
#include "Rendering/Light/PointLight.h"
#include "Components/TransformComponent.h"
#include "Components/ModelComponent.h"
#include "Core/EntityRegistry.h"
#include "Util/CollisionDetection.h"
#include "Apparatus.h"

void Level::init()
{

}

void Level::update(float dt)
{
	for (Entity* entity : spawnedEntities)
	{
		if (entity)
		{
			entity->update(dt);
		}
	}
}

Entity* Level::spawnEntity(const std::string& templateName)
{
    if (Entity* spawnedEntity = Apparatus::getEntityRegistry().createEntityFromTemplate(templateName))
    {
        spawnedEntity->init();
        spawnedEntity->onSpawn();

        spawnedEntities.push_back(std::move(spawnedEntity));

        return spawnedEntity;
    }

    return nullptr;
}

Entity* Level::findEntity(const std::string& name)
{
	auto searchIter = std::find_if(spawnedEntities.begin(), spawnedEntities.end(), [name](Entity* entity) {
		return entity && entity->getEntityName() == name;
		});

	if (searchIter != spawnedEntities.end())
	{
		return *searchIter;
	}

	return nullptr;
}

const Entity* Level::findEntity(const std::string& name) const
{
	auto searchIter = std::find_if(spawnedEntities.begin(), spawnedEntities.end(), [name](Entity* entity) {
		return entity && entity->getEntityName() == name;
		});

	if (searchIter != spawnedEntities.end())
	{
		return *searchIter;
	}

	return nullptr;
}

void Level::addEntity(Entity* entity)
{
	spawnedEntities.push_back(entity);
}

std::vector<RayTraceResult> Level::traceRay(const glm::vec3& origin, const glm::vec3& direction, DetectionType detectionType)
{
	if (detectionType != DetectionType::Visibility)
	{
		return {};
	}

	std::vector<RayTraceResult> result;
	for (Entity* entity : spawnedEntities)
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

			if (const Model* model = modelComponent->getModel())
			{
				for (Mesh* mesh : model->getMeshes())
				{
					auto intersections = rayVsMesh(origin, direction, mesh, modelComponent->getTransform(), modelComponent->getWorldPosition());
					for (const auto& intersectionPair : intersections)
					{
						RayTraceResult traceResult;
						traceResult.near = intersectionPair.first;
						traceResult.far = intersectionPair.second;
						traceResult.entity = entity;
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

	for (int i = 0; i < result.size(); ++i)
	{
		LOG(result[i].modelComponent->getModel()->getAssetName(), LogLevel::Info);
	}

	LOG("\n", LogLevel::Info);

	return result;
}

std::vector<Entity*>& Level::getAllEntities()
{
	return spawnedEntities;
}
