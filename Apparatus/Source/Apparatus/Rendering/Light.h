#pragma once

#include <glm/glm.hpp>

enum LightType
{
	DirectionalLight = 0,
	PointLight,
	SpotLight
};

class Light
{
public:
	Light(LightType type);
	Light(const Light& other);
	virtual ~Light() = default;

	LightType getType() const;
	void setType(LightType type);

	const glm::vec3& getColor() const;
	void setColor(const glm::vec3& color);

	void setDirection(const glm::vec3& direction);
	const glm::vec3& getDirection() const;

	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	void setRadius(float radius);
	const float& getRadius() const;

	void setCompression(float compression);
	const float& getCompression() const;

	void setCutOff(float cutOff);
	float getCutOff() const;

	void setOuterCutOff(float outerCutOff);
	float getOuterCutOff() const;

protected:
	LightType type;

	glm::vec3 color;
	glm::vec3 direction;
	glm::vec3 position;

	float radius;
	float compression;
	float cutOff;
	float outerCutOff;
};
