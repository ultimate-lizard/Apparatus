#pragma once

#include "Component.h"
#include "../Rendering/Light.h"

class PointLightComponent : public Component
{
public:
	PointLightComponent(Entity* owner);
	PointLightComponent(Entity* owner, const std::string& name);

	virtual void update(float dt) override;

	const glm::vec3& getAmbientColor() const;
	const glm::vec3& getDiffuseColor() const;
	const glm::vec3& getSpecularColor() const;

	void setAmbientColor(const glm::vec3& ambientColor);
	void setDiffuseColor(const glm::vec3& diffuseColor);
	void setSpecularColor(const glm::vec3& specularColor);

	PointLight& getPointLight();

protected:
	virtual void assignDefaultObjectName() override;

	PointLight pointLight;
};
