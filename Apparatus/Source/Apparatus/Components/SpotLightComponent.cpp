#include "SpotLightComponent.h"

#include "ModelComponent.h"
#include "TransformComponent.h"
#include "../Server/Entity.h"

SpotLightComponent::SpotLightComponent(Entity* owner) :
    Component(owner)
{
    assignDefaultObjectName();
}

SpotLightComponent::SpotLightComponent(Entity* owner, const std::string& name) :
    Component(owner, name)
{
    setObjectName(name);
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
			if (MaterialInstance* matInstance = modelInstance->getMaterialInstance(0))
			{
				MaterialParameters& params = matInstance->getMaterialParameters();
				params.setVec3("diffuse", getDiffuseColor());
			}
		}
	}

	if (TransformComponent* transform = owner->findComponent<TransformComponent>())
	{
		spotLight.setPosition(transform->getWorldPosition());
		spotLight.setDirection(transform->getForward());
	}
}

const glm::vec3& SpotLightComponent::getAmbientColor() const
{
	return spotLight.getAmbientColor();
}

const glm::vec3& SpotLightComponent::getDiffuseColor() const
{
	return spotLight.getDiffuseColor();
}

const glm::vec3& SpotLightComponent::getSpecularColor() const
{
	return spotLight.getSpecularColor();
}

void SpotLightComponent::setAmbientColor(const glm::vec3& ambientColor)
{
	spotLight.setAmbientColor(ambientColor);
}

void SpotLightComponent::setDiffuseColor(const glm::vec3& diffuseColor)
{
	spotLight.setDiffuseColor(diffuseColor);
}

void SpotLightComponent::setSpecularColor(const glm::vec3& specularColor)
{
	spotLight.setSpecularColor(specularColor);
}

SpotLight& SpotLightComponent::getSpotLight()
{
	return spotLight;
}

void SpotLightComponent::assignDefaultObjectName()
{
	setObjectName(owner->getObjectName() + "_SpotLightComponent");
}
