#pragma once

#include "Primitives.h"

class Model;
class Entity;

Box generateAABB(const Model* model, const glm::vec3& position, const glm::quat& orientation);

std::pair<glm::vec3, glm::vec3> rayVsAABB(const glm::vec3& position, const glm::vec3& direction, const Box& box);