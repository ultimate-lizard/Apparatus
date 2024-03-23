#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>

#include "../Util/Primitive.h"

void drawDebugPoint(const glm::vec3& position, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
void drawDebugLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
void drawDebugBox(const glm::vec3& size, const glm::vec3& position, const glm::vec4& color, float drawSize = 1.0f, bool persistent = false, float lifetime = 0.0f);
