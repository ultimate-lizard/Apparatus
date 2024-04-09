#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();

	void setDirection(const glm::vec3& direction);
	const glm::vec3& getDirection() const;

	void setCutOff(float cutOff);
	float getCutOff() const;

	void setOuterCutOff(float outerCutOff);
	float getOuterCutOff() const;

protected:
	glm::vec3 direction;

	float cutOff;
	float outerCutOff;
};
