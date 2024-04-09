#include "PointLight.h"

PointLight::PointLight() :
	position(0.0f),
	radius(10.0f),
	compression(1.0f)
{
}

PointLight::PointLight(const PointLight& other) :
	position(other.position),
	radius(other.radius),
	compression(other.compression)
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

void PointLight::setRadius(float radius)
{
	this->radius = radius;
}

const float& PointLight::getRadius() const
{
	return radius;
}

void PointLight::setCompression(float compression)
{
	this->compression = compression;
}

const float& PointLight::getCompression() const
{
	return compression;
}
