#pragma once

#include <vector>

#include "Component.h"
#include "../Util/Primitive.h"

class ModelComponent;

class CollisionComponent : public Component
{
public:
	CollisionComponent();
	CollisionComponent(const std::string& componentName);

	virtual void init() override;
	virtual void update(float dt) override {};

	std::vector<Box> getAllAABB() const;
	const Box* findAABB(ModelComponent* modelComponent) const;

	void regenerateAABB();

protected:
	std::vector<std::pair<Box, const ModelComponent*>> cachedAABBs;
};
