#pragma once

#include <glm/glm.hpp>

class Viewport
{
public:
	Viewport();
	Viewport(const glm::ivec2& position, const glm::ivec2& size);

	const glm::ivec2& getPosition() const;
	const glm::ivec2& getSize() const;

	void setPosition(const glm::ivec2 position);
	void setSize(const glm::ivec2 size);

	void update();

private:
	glm::ivec2 position;
	glm::ivec2 size;
};
