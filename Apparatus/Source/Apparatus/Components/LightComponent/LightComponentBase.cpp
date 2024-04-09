#include "LightComponentBase.h"

#include "../ModelComponent.h"
#include "../../Server/Entity.h"

LightComponentBase::LightComponentBase(const std::string& name) :
    Component(name),
    color(0.0)
{
}

void LightComponentBase::update(float dt)
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
				params.setVec3("diffuse", getColor());
			}
		}
	}
}

void LightComponentBase::setColor(const glm::vec3& color)
{
    this->color = color;
}

const glm::vec3& LightComponentBase::getColor() const
{
    return color;
}
