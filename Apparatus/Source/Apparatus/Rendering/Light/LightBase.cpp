#include "LightBase.h"

LightBase::LightBase(const LightBase& other) :
	color(other.color)
{
}

const glm::vec3& LightBase::getColor() const
{
	return color;
}

void LightBase::setColor(const glm::vec3& color)
{
	this->color = color;
}

LightBase::LightBase() :
	color(1.0f, 1.0f, 1.0f)
{
}
