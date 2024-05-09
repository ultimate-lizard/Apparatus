#include "LightComponent.h"

#include "ModelComponent.h"
#include "../Server/Entity.h"
#include "../Event/Event.h"
#include "../Apparatus.h"
#include "../Rendering/Material.h"

LightComponent::LightComponent(LightType type) :
	Component("LightComponent"),
	light(type)
{
}

LightComponent::LightComponent(const std::string& name, LightType type) :
    Component(name),
	light(type)
{
}

LightComponent::LightComponent(const LightComponent& other) :
	Component(other),
	SceneNode(other),
	light(other.light)
{

}

void LightComponent::calculateTransform()
{
	if (isDirty())
	{
		light.setPosition(getWorldPosition());
		light.setDirection(getForward());
	}

	SceneNode::calculateTransform();
}

std::unique_ptr<Component> LightComponent::clone()
{
	return std::make_unique<LightComponent>(*this);
}

void LightComponent::init()
{
	Apparatus::getEventDispatcher().dispatch(std::make_shared<LightComponentCreationEvent>(this));
}

void LightComponent::update(float dt)
{
	Component::update(dt);

	if (!owner)
	{
		return;
	}

	if (ModelComponent* modelComponent = owner->findComponent<ModelComponent>())
	{
		if (Model* model = modelComponent->getModel())
		{
			size_t materialIndex = 0;
			if (getType() == LightType::SpotLight)
			{
				materialIndex = 1;
			}

			if (Material* materialSlot = model->getMaterialSlot(materialIndex))
			{
				MaterialParameters& params = materialSlot->getParameters();
				params.setVec3("material.color", getColor());
			}
		}
	}
}

Light& LightComponent::getLight()
{
	return light;
}

void LightComponent::setType(LightType type)
{
	light.setType(type);
}

LightType LightComponent::getType() const
{
	return light.getType();
}

void LightComponent::setColor(const glm::vec3& color)
{
	light.setColor(color);
}

const glm::vec3& LightComponent::getColor() const
{
	return light.getColor();
}

void LightComponent::setDirection(const glm::vec3& direction)
{
	light.setDirection(direction);
}

const glm::vec3& LightComponent::getDirection() const
{
	return light.getDirection();
}

void LightComponent::setPosition(const glm::vec3& position)
{
	light.setPosition(position);
}

const glm::vec3& LightComponent::getPosition() const
{
	return light.getPosition();
}

void LightComponent::setRadius(float radius)
{
	light.setRadius(radius);
}

const float& LightComponent::getRadius() const
{
	return light.getRadius();
}

void LightComponent::setCompression(float compression)
{
	light.setCompression(compression);
}

const float& LightComponent::getCompression() const
{
	return light.getCompression();
}

void LightComponent::setCutOff(float cutOff)
{
	light.setCutOff(cutOff);
}

float LightComponent::getCutOff() const
{
	return light.getCutOff();
}

void LightComponent::setOuterCutOff(float outerCutOff)
{
	light.setOuterCutOff(outerCutOff);
}

float LightComponent::getOuterCutOff() const
{
	return light.getOuterCutOff();
}
