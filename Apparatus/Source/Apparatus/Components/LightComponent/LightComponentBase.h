#pragma once

#include <glm/glm.hpp>

#include "../Component.h"

class LightBase;

class LightComponentBase : public Component
{
public:
	LightComponentBase(const std::string& name);

	virtual void update(float dt) override;

	virtual void setColor(const glm::vec3& color);
	const glm::vec3& getColor() const;

protected:
	glm::vec3 color;
};
