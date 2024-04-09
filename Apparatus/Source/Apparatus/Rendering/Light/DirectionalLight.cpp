#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() :
	direction(0.0f, 0.0f, 1.0f)
{
}

void DirectionalLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}

const glm::vec3& DirectionalLight::getDirection() const
{
	return direction;
}
