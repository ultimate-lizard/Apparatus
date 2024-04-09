#include "LightBase.h"

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
