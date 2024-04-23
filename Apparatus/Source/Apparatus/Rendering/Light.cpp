#include "Light.h"

Light::Light(LightType type) :
	type(type),
	color(1.0f, 1.0f, 1.0f),
	direction(0.0f, 0.0f, 1.0f),
	position(0.0f),
	radius(5.0f),
	compression(1.0f),
	cutOff(12.5f),
	outerCutOff(15.5f)
{
}

Light::Light(const Light& other) :
	type(other.type),
	color(other.color),
	direction(other.direction),
	position(other.position),
	radius(other.radius),
	compression(other.compression),
	cutOff(other.cutOff),
	outerCutOff(other.outerCutOff)
{
}

LightType Light::getType() const
{
	return type;
}

void Light::setType(LightType type)
{
	this->type = type;
}

const glm::vec3& Light::getColor() const
{
	return color;
}

void Light::setColor(const glm::vec3& color)
{
	this->color = color;
}

void Light::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}

const glm::vec3& Light::getDirection() const
{
	return direction;
}

void Light::setPosition(const glm::vec3& position)
{
	this->position = position;
}

const glm::vec3& Light::getPosition() const
{
	return position;
}

void Light::setRadius(float radius)
{
	this->radius = radius;
}

const float& Light::getRadius() const
{
	return radius;
}

void Light::setCompression(float compression)
{
	this->compression = compression;
}

const float& Light::getCompression() const
{
	return compression;
}

void Light::setCutOff(float cutOff)
{
	this->cutOff = cutOff;
}

float Light::getCutOff() const
{
	return cutOff;
}

void Light::setOuterCutOff(float outerCutOff)
{
	this->outerCutOff = outerCutOff;
}

float Light::getOuterCutOff() const
{
	return outerCutOff;
}
