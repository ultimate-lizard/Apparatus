#pragma once

#include <vector>
#include <memory>
#include <map>

#include "Server.h"
#include "Entity.h"
#include "../Components/Component.h"
#include "../Rendering/Model.h"
#include "../Rendering/Shader.h"
#include "../Common/Primitives.h"

class CollisionComponent;

struct RayTraceResult
{
	glm::vec3 near;
	glm::vec3 far;
	Entity* entity;
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
	virtual void update(float dt) override;

	Entity* createEntity(const std::string& id);
	Entity* findEntity(const std::string& id);
	std::vector<std::unique_ptr<Entity>>& getAllEntities();

	template <class ComponentType, typename ... Args>
	ComponentType* createComponent(Entity* owner, Args ... args);

	static void drawPoint(const glm::vec3& position, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
	static void drawLine(const Line& line, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
	static void drawRectangle(const Box& box, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);

	template <typename PrimitiveType>
	void updateDebugPrimitiveVector(std::vector<std::pair<PrimitiveType, DebugPrimitiveData>>& primitives, float dt)
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

	const DebugPrimitives& getDebugPrimitives() const;

	std::vector<RayTraceResult> traceRay(const glm::vec3& direction, const glm::vec3& position, float length);

protected:
	void createEntities();

	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Component>> components;

	static DebugPrimitives debugPrimitives;
};

template<class ComponentType, typename ...Args>
inline ComponentType* LocalServer::createComponent(Entity* owner, Args ...args)
{
	components.push_back(std::make_unique<ComponentType>(args)...);
	ComponentType* result = dynamic_cast<ComponentType*>(components.back().get());
	result->setOwner(owner);
	owner->addComponent(result);
	return result;
}
