#pragma once

#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Rendering/Rotator.h"

class Entity;
class ModelComponent;

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

class Level
{
public:
	Level() = default;
	~Level() = default;

	Level(const Level&) = delete;
	Level(Level&&) = delete;

	void init();
	void update(float dt);

	Entity* spawnEntity(const std::string& templateName);
	Entity* spawnEntity(const std::string& templateName, const glm::vec3& position, const Rotator& rotator);
	Entity* findEntity(const std::string& name);
	const Entity* findEntity(const std::string& name) const;
	void addEntity(Entity* entity);

	// Returns an unsorted vector of entities and near and far positions of the intersections
	std::vector<RayTraceResult> traceRay(const glm::vec3& origin, const glm::vec3& direction, DetectionType detectionType = DetectionType::Visibility);

	std::vector<Entity*>& getAllEntities();

protected:
	std::vector<Entity*> spawnedEntities;
};
