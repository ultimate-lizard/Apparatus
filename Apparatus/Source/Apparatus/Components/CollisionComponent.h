#pragma once

#include <vector>

#include "Component.h"
#include "../Common/Primitives.h"

class ModelComponent;

class CollisionComponent : public Component
{
public:
	CollisionComponent(const std::string& id);

	virtual void init() override;
	virtual void update(float dt) override {};

	std::vector<Box> getAllAABB() const;
	const Box* findAABB(ModelComponent* modelComponent) const;

	void regenerateAABB();

protected:
	// void generateAABB();

	std::vector<std::pair<Box, const ModelComponent*>> cachedAABBs;
};
