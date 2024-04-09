#include "CollisionComponent.h"

#include "ModelComponent.h"
#include "../Rendering/Model.h"
#include "../Rendering/Mesh.h"
#include "../Server/Entity.h"
#include "../Util/BoundingBox.h"

CollisionComponent::CollisionComponent() :
	Component("CollisionComponent")
{
	
}

CollisionComponent::CollisionComponent(const std::string& componentName) :
	Component(componentName)
{
}

void CollisionComponent::init()
{
	Component::init();

	if (!owner)
	{
		return;
	}

	regenerateAABB();
}

std::vector<Box> CollisionComponent::getAllAABB() const
{
	std::vector<Box> result;

	for (const auto& aabbPair : cachedAABBs)
	{
		result.push_back(aabbPair.first);
	}

	return result;
}

const Box* CollisionComponent::findAABB(ModelComponent* modelComponent) const
{
	for (const auto& aabbPair : cachedAABBs)
	{
		if (aabbPair.second == modelComponent)
		{
			return &aabbPair.first;
		}
	}

	return nullptr;
}

void CollisionComponent::regenerateAABB()
{
	cachedAABBs.clear();

	if (owner)
	{
		for (ModelComponent* modelComponent : owner->getAllComponentsOfClass<ModelComponent>())
		{
			Box newBox = generateAABB(modelComponent->getModel(), modelComponent->getWorldPosition(), modelComponent->getWorldOrientation(), modelComponent->getWorldScale());
			cachedAABBs.push_back({ newBox, modelComponent });
		}
	}
}
