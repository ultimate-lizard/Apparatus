#include "PointLightComponent.h"

#include "../ModelComponent.h"
#include "../TransformComponent.h"
#include "../../Server/Entity.h"
#include "../../Rendering/Renderer.h"
#include "../../Event/EventDispatcher.h"
#include "../../Apparatus.h"

PointLightComponent::PointLightComponent() :
    LightComponentBase("PointLightComponent")
{

}

PointLightComponent::PointLightComponent(const std::string& componentName) :
	LightComponentBase(componentName)
{
}

PointLightComponent::PointLightComponent(const PointLightComponent& other) :
	LightComponentBase(other),
	SceneNode(other),
	pointLight(other.pointLight)
{

}

std::unique_ptr<Component> PointLightComponent::clone()
{
	return std::make_unique<PointLightComponent>(*this);
}

void PointLightComponent::init()
{
	LightComponentBase::init();

	auto lightEvent = std::make_shared<LightComponentCreationEvent>();
	lightEvent->pointLightComponent = this;
	Apparatus::getEventDispatcher().dispatch(lightEvent);
}

void PointLightComponent::update(float dt)
{
	LightComponentBase::update(dt);

	if (!owner)
	{
		return;
	}

	pointLight.setPosition(getWorldPosition());
}

void PointLightComponent::onEntitySpawn()
{
	LightComponentBase::onEntitySpawn();

	if (TransformComponent* transform = owner->findComponent<TransformComponent>())
	{

	}
}

void PointLightComponent::setColor(const glm::vec3& color)
{
	LightComponentBase::setColor(color);

	pointLight.setColor(color);
}

void PointLightComponent::setRadius(float radius)
{
	pointLight.setRadius(radius);
}

float PointLightComponent::getRadius() const
{
	return pointLight.getRadius();
}

void PointLightComponent::setCompression(float compression)
{
	pointLight.setCompression(compression);
}

float PointLightComponent::getCompression() const
{
	return pointLight.getCompression();
}

PointLight& PointLightComponent::getPointLight()
{
	return pointLight;
}
