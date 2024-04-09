#include "SpotLightComponent.h"

#include "../ModelComponent.h"
#include "../TransformComponent.h"
#include "../../Server/Entity.h"

SpotLightComponent::SpotLightComponent() :
	LightComponentBase("SpotLightComponent")
{

}

SpotLightComponent::SpotLightComponent(const std::string& componentName) :
	LightComponentBase(componentName)
{
}

std::unique_ptr<Component> SpotLightComponent::clone()
{
	std::unique_ptr<SpotLightComponent> newSpotLightComponent = std::make_unique<SpotLightComponent>(getComponentName());

	if (newSpotLightComponent)
	{
		newSpotLightComponent->spotLight = spotLight;
	}

	return newSpotLightComponent;
}

void SpotLightComponent::update(float dt)
{
	LightComponentBase::update(dt);

	if (!owner)
	{
		return;
	}

	if (TransformComponent* transform = owner->findComponent<TransformComponent>())
	{
		spotLight.setPosition(transform->getWorldPosition());
		spotLight.setDirection(transform->getForward());
	}
}

void SpotLightComponent::setColor(const glm::vec3& color)
{
	LightComponentBase::setColor(color);

	spotLight.setColor(color);
}

SpotLight& SpotLightComponent::getSpotLight()
{
	return spotLight;
}
