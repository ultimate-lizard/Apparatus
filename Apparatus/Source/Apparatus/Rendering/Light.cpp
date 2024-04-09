#include "Light.h"

LightBase::LightBase() :
	ambient(1.0f, 1.0f, 1.0f),
	diffuse(1.0f, 1.0f, 1.0f),
	specular(1.0f, 1.0f, 1.0f)
{
}

const glm::vec3& LightBase::getAmbientColor() const
{
	return ambient;
}

const glm::vec3& LightBase::getDiffuseColor() const
{
	return diffuse;
}

const glm::vec3& LightBase::getSpecularColor() const
{
	return specular;
}

void LightBase::setAmbientColor(const glm::vec3& ambientColor)
{
	ambient = ambientColor;
}

void LightBase::setDiffuseColor(const glm::vec3& diffuseColor)
{
	diffuse = diffuseColor;
}

void LightBase::setSpecularColor(const glm::vec3& specularColor)
{
	specular = specularColor;
}

PointLight::PointLight() :
	position(0.0f),
	constant(1.0f),
	linear(0.09f),
	quadratic(0.032f)
{
}

void PointLight::setPosition(const glm::vec3& position)
{
	this->position = position;
}

const glm::vec3& PointLight::getPosition() const
{
	return position;
}

float PointLight::getConstant() const
{
	return constant;
}

float PointLight::getLinear() const
{
	return linear;
}

float PointLight::getQuadratic() const
{
	return quadratic;
}

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

SpotLight::SpotLight() :
	position(0.0f),
	direction({0.0f, 0.0f, 1.0f}),
	cutOff(12.5f),
	outerCutOff(15.5f),
	constant(1.0f),
	linear(0.09f),
	quadratic(0.032f)
{

}

void SpotLight::setPosition(const glm::vec3& position)
{
	this->position = position;
}

const glm::vec3& SpotLight::getPosition() const
{
	return position;
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

float SpotLight::getConstant() const
{
	return constant;
}

float SpotLight::getLinear() const
{
	return linear;
}

float SpotLight::getQuadratic() const
{
	return quadratic;
}