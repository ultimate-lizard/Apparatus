#include "CollisionComponent.h"

#include "ModelComponent.h"
#include "../Rendering/Model.h"
#include "../Rendering/Mesh.h"
#include "../Server/Entity.h"
#include "../Common/BoundingBox.h"

CollisionComponent::CollisionComponent(const std::string& id) :
	Component(id)
{

}

void CollisionComponent::init()
{
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

	std::vector<ModelComponent*> modelComponents = owner->getAllComponentsOfClass<ModelComponent>();
	for (ModelComponent* modelComponent : modelComponents)
	{
		Box newBox = generateAABB(modelComponent->getModel(), modelComponent->getDerivedPosition(), modelComponent->getDerivedOrientation());
		cachedAABBs.push_back({ newBox, modelComponent });
	}
}
