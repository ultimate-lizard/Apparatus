#include "PointLightComponent.h"

#include "ModelComponent.h"
#include "TransformComponent.h"
#include "../Server/Entity.h"

PointLightComponent::PointLightComponent(Entity* owner) :
    Component(owner)
{
    assignDefaultObjectName();
}

PointLightComponent::PointLightComponent(Entity* owner, const std::string& name) :
    Component(owner, name)
{
    setObjectName(name);
}

void PointLightComponent::update(float dt)
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
		pointLight.setPosition(transform->getWorldPosition());
	}
}

const glm::vec3& PointLightComponent::getAmbientColor() const
{
    return pointLight.getAmbientColor();
}

const glm::vec3& PointLightComponent::getDiffuseColor() const
{
    return pointLight.getDiffuseColor();
}

const glm::vec3& PointLightComponent::getSpecularColor() const
{
    return pointLight.getSpecularColor();
}

void PointLightComponent::setAmbientColor(const glm::vec3& ambientColor)
{
    pointLight.setAmbientColor(ambientColor);
}

void PointLightComponent::setDiffuseColor(const glm::vec3& diffuseColor)
{
    pointLight.setDiffuseColor(diffuseColor);
}

void PointLightComponent::setSpecularColor(const glm::vec3& specularColor)
{
    pointLight.setSpecularColor(specularColor);
}

PointLight& PointLightComponent::getPointLight()
{
    return pointLight;
}

void PointLightComponent::assignDefaultObjectName()
{
    setObjectName(owner->getObjectName() + "_PointLightComponent");
}
