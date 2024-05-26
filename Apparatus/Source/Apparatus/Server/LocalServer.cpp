#include "LocalServer.h"

#include <glm/gtc/quaternion.hpp>

#include "../Apparatus.h"
#include "../Core/Level.h"
#include "../Core/Logger.h"
#include "../Rendering/Material.h"
#include "../Util/CollisionDetection.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/ModelComponent.h"

DebugPrimitives LocalServer::debugPrimitives;

LocalServer::LocalServer() :
	Server(),
	level(nullptr)
{
}

void LocalServer::init()
{
	Apparatus& app = Apparatus::get();
	level = app.getLevel();
}

void LocalServer::update(float dt)
{
	// DEBUG INFO
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
	/////////////

	if (level)
	{
		for (Entity* entity : level->getAllEntities())
		{
			if (entity)
			{
				entity->update(dt);
			}
		}
	}
}

void LocalServer::connect(const ConnectionInfo& info)
{
	if (level)
	{
		if (Entity* player = level->findEntity("Player"))
		{
			if (Client* client = info.client)
			{
				client->setActiveEntity(player);
				// TODO: Add more info to the comment
				LOG("A player has possessed an entity!", LogLevel::Info);
			}
		}
	}
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
