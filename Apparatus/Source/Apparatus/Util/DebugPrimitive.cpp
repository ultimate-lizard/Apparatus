#include "DebugPrimitive.h"

#include "../Server/LocalServer.h"

void drawDebugPoint(const glm::vec3& position, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	LocalServer::drawDebugPoint(position, color, drawSize, persistent, lifetime);
}

void drawDebugLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	LocalServer::drawDebugLine({ start, end }, color, drawSize, persistent, lifetime);
}

void drawDebugBox(const glm::vec3& size, const glm::vec3& position, const glm::vec4& color, float drawSize, bool persistent, float lifetime)
{
	LocalServer::drawDebugBox({ size, position }, color, drawSize, persistent, lifetime);
}
