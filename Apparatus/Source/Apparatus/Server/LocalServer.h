#pragma once

#include <vector>
#include <memory>
#include <map>

#include "Server.h"
#include "Entity.h"
#include "../Components/Component.h"
#include "../Rendering/Model.h"
#include "../Rendering/Shader.h"
#include "../Util/DebugPrimitive.h"

class CollisionComponent;
class ModelComponent;
class EntityRegistry;
class Level;

struct DebugPrimitiveData
{
	glm::vec4 color{ 0.0f };
	float lifeTime = 0.0f;
	unsigned int vertexCount = 1;
};

struct DebugPrimitives
{
	// Index: Draw Size
	std::map<float, std::vector<std::pair<Point, DebugPrimitiveData>>> debugPoints;
	std::map<float, std::vector<std::pair<Line, DebugPrimitiveData>>> debugLines;
	std::map<float, std::vector<std::pair<Box, DebugPrimitiveData>>> debugBoxes;
};

class LocalServer : public Server
{
public:
	LocalServer();
	virtual ~LocalServer() = default;

	LocalServer(const LocalServer&) = delete;
	LocalServer(LocalServer&&) = delete;
	void operator=(const LocalServer&) = delete;

	virtual void init();
	virtual void start() {};
	virtual void update(float dt) override;

	virtual void connect(const ConnectionInfo& info) override;

	static void drawDebugPoint(const glm::vec3& position, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
	static void drawDebugLine(const Line& line, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
	static void drawDebugBox(const Box& box, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);

	const DebugPrimitives& getDebugPrimitives() const;

protected:
	Level* level;

private:
	template <typename PrimitiveType>
	void updateDebugPrimitiveVector(std::vector<std::pair<PrimitiveType, DebugPrimitiveData>>& primitives, float dt);

	static DebugPrimitives debugPrimitives;
};

template <typename PrimitiveType>
inline void LocalServer::updateDebugPrimitiveVector(std::vector<std::pair<PrimitiveType, DebugPrimitiveData>>& primitives, float dt)
{
	for (auto iter = primitives.begin(); iter != primitives.end();)
	{
		if (iter->second.lifeTime == -1.0f)
		{
			++iter;
			continue;
		}
		else if (iter->second.lifeTime <= 0.0f)
		{
			iter = primitives.erase(iter);
		}
		else
		{
			iter->second.lifeTime -= dt;
			++iter;
		}
	}
}
