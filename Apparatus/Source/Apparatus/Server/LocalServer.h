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

struct RayTraceResult
{
	glm::vec3 near;
	glm::vec3 far;
	Entity* entity;
	ModelComponent* modelComponent;
};

enum DetectionType
{
	Visibility
};

class LocalServer : public Server
{
public:
	LocalServer(Apparatus* apparatus);
	virtual ~LocalServer() = default;

	LocalServer(const LocalServer&) = delete;
	LocalServer(LocalServer&&) = delete;
	void operator=(const LocalServer&) = delete;

	virtual void init() override;
	virtual void start() override {};
	virtual void update(float dt) override;

	Entity* createEntity(const std::string& id);
	Entity* findEntity(const std::string& id);
	std::vector<std::unique_ptr<Entity>>& getAllEntities();

	template <class ComponentType, typename ... Args>
	ComponentType* createComponent(Args&& ... args);

	static void drawDebugPoint(const glm::vec3& position, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
	static void drawDebugLine(const Line& line, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
	static void drawDebugBox(const Box& box, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);

	template <typename PrimitiveType>
	void updateDebugPrimitiveVector(std::vector<std::pair<PrimitiveType, DebugPrimitiveData>>& primitives, float dt);

	const DebugPrimitives& getDebugPrimitives() const;

	// Returns an unsorted vector of entities and near and far positions of the intersections
	std::vector<RayTraceResult> traceRay(const glm::vec3& origin, const glm::vec3& direction, DetectionType detectionType = DetectionType::Visibility);
	glm::vec3 getCursorToWorldRay(const glm::mat4& view, const glm::mat4& projection);

protected:
	virtual void assignDefaultObjectName() override;

	virtual void setupEntities();

	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Component>> components;

	static DebugPrimitives debugPrimitives;
};

template<class ComponentType, typename ...Args>
inline ComponentType* LocalServer::createComponent(Args&& ... args)
{
	components.push_back(std::make_unique<ComponentType>(std::forward<Args>(args)...));
	ComponentType* result = dynamic_cast<ComponentType*>(components.back().get());
	Entity* owner = result->getOwner();
	owner->addComponent(result);
	return result;
}

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
