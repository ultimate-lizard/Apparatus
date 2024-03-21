#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "../Rendering/Mesh.h"

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

struct DebugPrimitiveData
{
	glm::vec4 color { 0.0f };
	float lifeTime = 0.0f;
	unsigned int vertexCount = 1;
};

struct DebugPrimitives
{
	// Index: Draw Size
	std::map<float, std::vector<std::pair<Point, DebugPrimitiveData>>> debugPoints;
	std::map<float, std::vector<std::pair<Line, DebugPrimitiveData>>> debugLines;
	std::map<float, std::vector<std::pair<Box, DebugPrimitiveData>>> debugBoxes;
};
