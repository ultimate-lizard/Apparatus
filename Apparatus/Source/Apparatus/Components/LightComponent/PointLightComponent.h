#pragma once

#include <memory>

#include "LightComponentBase.h"
#include "../../Rendering/SceneNode.h"
#include "../../Rendering/Light/PointLight.h"

class PointLightComponent : public LightComponentBase, public SceneNode
{
public:
	PointLightComponent();
	PointLightComponent(const std::string& componentName);
	PointLightComponent(const PointLightComponent& other);

	virtual std::unique_ptr<Component> clone() override;

	virtual void init() override;
	virtual void update(float dt) override;
	virtual void onEntitySpawn() override;

	virtual void setColor(const glm::vec3& color) override;

	void setRadius(float radius);
	float getRadius() const;

	void setCompression(float compression);
	float getCompression() const;

	PointLight& getPointLight();

protected:
	PointLight pointLight;
};
