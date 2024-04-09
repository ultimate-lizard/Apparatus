#pragma once

#include "LightBase.h"

class DirectionalLight : public LightBase
{
public:
	DirectionalLight();

	void setDirection(const glm::vec3& direction);
	const glm::vec3& getDirection() const;

protected:
	glm::vec3 direction;
};
