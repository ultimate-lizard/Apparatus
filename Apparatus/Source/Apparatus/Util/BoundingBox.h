#pragma once

#include <vector>

#include "Primitive.h"

class Model;

Box generateAABB(const Model* model, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale);
Box generateAABB(const std::vector<Model*>& models, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale);
