#include "SpotLight.h"

SpotLight::SpotLight() :
	direction({ 0.0f, 0.0f, 1.0f }),
	cutOff(12.5f),
	outerCutOff(15.5f)
{

}

void SpotLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}

const glm::vec3& SpotLight::getDirection() const
{
	return direction;
}

void SpotLight::setCutOff(float cutOff)
{
	this->cutOff = cutOff;
}

float SpotLight::getCutOff() const
{
	return cutOff;
}

void SpotLight::setOuterCutOff(float outerCutOff)
{
	this->outerCutOff = outerCutOff;
}

float SpotLight::getOuterCutOff() const
{
	return outerCutOff;
}
