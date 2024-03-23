#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Primitive.h"

class Mesh;

std::pair<glm::vec3, glm::vec3> rayVsAABB(const glm::vec3& position, const glm::vec3& direction, const Box& box);

// Returns a vector of near and far intersections of the mesh
std::vector<std::pair<glm::vec3, glm::vec3>> rayVsMesh(const glm::vec3& origin, const glm::vec3& direction, float length, const Mesh* mesh, const glm::mat4& meshTransform);

glm::vec3 rayVsPolygon(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
