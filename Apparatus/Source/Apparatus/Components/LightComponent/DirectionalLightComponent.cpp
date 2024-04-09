#include "DirectionalLightComponent.h"

#include "../ModelComponent.h"
#include "../TransformComponent.h"
#include "../../Server/Entity.h"

DirectionalLightComponent::DirectionalLightComponent() :
	LightComponentBase("DirectionalLightComponent")
{

}

DirectionalLightComponent::DirectionalLightComponent(const std::string& componentName) :
	LightComponentBase(componentName)
{
}

std::unique_ptr<Component> DirectionalLightComponent::clone()
{
	std::unique_ptr<DirectionalLightComponent> newDirectionalLightComponent = std::make_unique<DirectionalLightComponent>(getComponentName());

	if (newDirectionalLightComponent)
	{
		DirectionalLight& directionalLight = newDirectionalLightComponent->getDirectionalLight();
		DirectionalLight& newDirectionalLight = newDirectionalLightComponent->getDirectionalLight();

		newDirectionalLight.setColor(directionalLight.getColor());
		newDirectionalLight.setDirection(directionalLight.getDirection());
	}

	return std::move(newDirectionalLightComponent);
}

void DirectionalLightComponent::onEntitySpawn()
{
	LightComponentBase::onEntitySpawn();

	
}

void DirectionalLightComponent::update(float dt)
{
	LightComponentBase::update(dt);

	if (TransformComponent* transform = owner->findComponent<TransformComponent>())
	{
		directionalLight.setDirection(transform->getForward());
	}
}

void DirectionalLightComponent::setColor(const glm::vec3& color)
{
	LightComponentBase::setColor(color);

	directionalLight.setColor(color);
}

DirectionalLight& DirectionalLightComponent::getDirectionalLight()
{
	return directionalLight;
}
