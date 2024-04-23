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

SpotLightComponent::SpotLightComponent(const SpotLightComponent& other) :
	LightComponentBase(other),
	SceneNode(other),
	spotLight(spotLight)
{
}

std::unique_ptr<Component> SpotLightComponent::clone()
{
	return std::make_unique<SpotLightComponent>(getComponentName());
}

void SpotLightComponent::update(float dt)
{
	Component::update(dt);

	if (!owner)
	{
		return;
	}

	if (ModelComponent* modelComponent = owner->findComponent<ModelComponent>())
	{
		if (ModelInstance* modelInstance = modelComponent->getModelInstance())
		{
			if (MaterialInstance* matInstance = modelInstance->getMaterialInstance(1))
			{
				MaterialParameters& params = matInstance->getMaterialParameters();
				params.setVec3("diffuse", getColor());
			}
		}
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
