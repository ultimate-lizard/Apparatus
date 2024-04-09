#pragma once

#include "../LightComponent/LightComponentBase.h"
#include "../../Rendering/Light/DirectionalLight.h"

class DirectionalLightComponent : public LightComponentBase
{
public:
	DirectionalLightComponent();
	DirectionalLightComponent(const std::string& componentName);

	virtual std::unique_ptr<Component> clone() override;

	virtual void onEntitySpawn() override;
	virtual void update(float dt) override;

	virtual void setColor(const glm::vec3& color) override;

	DirectionalLight& getDirectionalLight();

protected:
	DirectionalLight directionalLight;
};
