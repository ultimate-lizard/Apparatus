#pragma once

#include <vector>

#include "Component.h"
#include "../Util/Primitive.h"

class ModelComponent;

class CollisionComponent : public Component
{
public:
	CollisionComponent(Entity* owner);

	virtual void init() override;
	virtual void update(float dt) override {};

	std::vector<Box> getAllAABB() const;
	const Box* findAABB(ModelComponent* modelComponent) const;

	void regenerateAABB();

protected:
	virtual void assignDefaultObjectName() override;

	std::vector<std::pair<Box, const ModelComponent*>> cachedAABBs;
};
