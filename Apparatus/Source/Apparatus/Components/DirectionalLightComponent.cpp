#include "DirectionalLightComponent.h"

#include "ModelComponent.h"
#include "TransformComponent.h"
#include "../Server/Entity.h"

DirectionalLightComponent::DirectionalLightComponent(Entity* owner) :
	Component(owner)
{
	assignDefaultObjectName();
}

DirectionalLightComponent::DirectionalLightComponent(Entity* owner, const std::string& name) :
	Component(owner, name)
{
	setObjectName(name);
}

void DirectionalLightComponent::update(float dt)
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
		directionalLight.setDirection(transform->getForward());
	}
}

const glm::vec3& DirectionalLightComponent::getAmbientColor() const
{
	return directionalLight.getAmbientColor();
}

const glm::vec3& DirectionalLightComponent::getDiffuseColor() const
{
	return directionalLight.getDiffuseColor();
}

const glm::vec3& DirectionalLightComponent::getSpecularColor() const
{
	return directionalLight.getSpecularColor();
}

void DirectionalLightComponent::setAmbientColor(const glm::vec3& ambientColor)
{
	directionalLight.setAmbientColor(ambientColor);
}

void DirectionalLightComponent::setDiffuseColor(const glm::vec3& diffuseColor)
{
	directionalLight.setDiffuseColor(diffuseColor);
}

void DirectionalLightComponent::setSpecularColor(const glm::vec3& specularColor)
{
	directionalLight.setSpecularColor(specularColor);
}

DirectionalLight& DirectionalLightComponent::getDirectionalLight()
{
	return directionalLight;
}

void DirectionalLightComponent::assignDefaultObjectName()
{
	setObjectName(owner->getObjectName() + "_DirectionalLightComponent");
}
