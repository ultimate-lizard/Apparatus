#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	Component("TransformComponent")
{
	
}

TransformComponent::TransformComponent(const std::string& componentName) :
	Component(componentName)
{
}

TransformComponent::TransformComponent(const TransformComponent& other) :
	Component(other),
	SceneNode(other)
{
}

std::unique_ptr<Component> TransformComponent::clone()
{
	std::unique_ptr<TransformComponent> newTransformComponentPtr = std::make_unique<TransformComponent>(*this);
	return newTransformComponentPtr;
}

void TransformComponent::update(float dt)
{
	calculateTransform();
}
