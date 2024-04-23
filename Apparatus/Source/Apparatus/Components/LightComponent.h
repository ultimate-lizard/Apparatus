#pragma once

#include <glm/glm.hpp>

#include "Component.h"
#include "../Rendering/Light.h"
#include "../Rendering/SceneNode.h"

class LightComponent : public Component, public SceneNode
{
public:
	LightComponent(LightType type);
	LightComponent(const std::string& name, LightType type);
	LightComponent(const LightComponent& other);
	virtual ~LightComponent() = default;

	virtual void calculateTransform() override;

	virtual std::unique_ptr<Component> clone() override;

	virtual void init() override;
	virtual void update(float dt) override;

	Light& getLight();

	void setType(LightType type);
	LightType getType() const;

	void setColor(const glm::vec3& color);
	const glm::vec3& getColor() const;

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
	Light light;
};
