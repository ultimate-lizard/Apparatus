#pragma once

#include <glm/glm.hpp>

struct Point
{
	glm::vec3 position {};
};

struct Line
{
	glm::vec3 start {};
	glm::vec3 end {};
};

struct Box
{
	glm::vec3 size {};
	glm::vec3 position {};
};
