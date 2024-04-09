#pragma once

#include <glm/glm.hpp>

class Level;

class LightBase
{
public:
	virtual ~LightBase() = default;

	const glm::vec3& getColor() const;
	void setColor(const glm::vec3& color);

protected:
	LightBase();

	glm::vec3 color;
};
