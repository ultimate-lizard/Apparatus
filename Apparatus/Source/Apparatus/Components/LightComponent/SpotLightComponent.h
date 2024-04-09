#pragma once

#include "LightComponentBase.h"
#include "../../Rendering/Light/SpotLight.h"

class SpotLightComponent : public LightComponentBase
{
public:
	SpotLightComponent();
	SpotLightComponent(const std::string& componentName);

	virtual std::unique_ptr<Component> clone() override;

	virtual void update(float dt) override;

	virtual void setColor(const glm::vec3& color) override;

	SpotLight& getSpotLight();

protected:
	SpotLight spotLight;
};
