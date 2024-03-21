#include "Viewport.h"

#include <glad/glad.h>

Viewport::Viewport() :
	position(0, 0),
	size(800, 600)
{

}

Viewport::Viewport(const glm::ivec2& position, const glm::ivec2& size) :
	position(position),
	size(size)
{

}

const glm::ivec2& Viewport::getPosition() const
{
	return position;
}

const glm::ivec2& Viewport::getSize() const
{
	return size;
}

void Viewport::setPosition(const glm::ivec2 position)
{
	this->position = position;

	update();
}

void Viewport::setSize(const glm::ivec2 size)
{
	this->size = size;

	update();
}

void Viewport::update()
{
	glViewport(position.x, position.y, size.x, size.y);
}
