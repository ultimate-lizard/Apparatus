#pragma once

#include "LightBase.h"

class PointLight : public LightBase
{
public:
	PointLight();
	PointLight(const PointLight& other);

	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	void setRadius(float radius);
	const float& getRadius() const;

	void setCompression(float compression);
	const float& getCompression() const;

protected:
	glm::vec3 position;

	float radius;
	float compression;
};
