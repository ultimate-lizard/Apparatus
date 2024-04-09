#pragma once

#include <glm/glm.hpp>

class LightBase
{
public:
	virtual ~LightBase() = default;

	const glm::vec3& getAmbientColor() const;
	const glm::vec3& getDiffuseColor() const;
	const glm::vec3& getSpecularColor() const;

	void setAmbientColor(const glm::vec3& ambientColor);
	void setDiffuseColor(const glm::vec3& diffuseColor);
	void setSpecularColor(const glm::vec3& specularColor);

protected:
	LightBase();

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class PointLight : public LightBase
{
public:
	PointLight();

	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	float getConstant() const;
	float getLinear() const;
	float getQuadratic() const;

protected:
	glm::vec3 position;
	
	float constant;
	float linear;
	float quadratic;
};

class DirectionalLight : public LightBase
{
public:
	DirectionalLight();

	void setDirection(const glm::vec3& direction);
	const glm::vec3& getDirection() const;

protected:
	glm::vec3 direction;
};

class SpotLight : public LightBase
{
public:
	SpotLight();

	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	void setDirection(const glm::vec3& direction);
	const glm::vec3& getDirection() const;

	void setCutOff(float cutOff);
	float getCutOff() const;

	void setOuterCutOff(float outerCutOff);
	float getOuterCutOff() const;

	float getConstant() const;
	float getLinear() const;
	float getQuadratic() const;

protected:
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
};
