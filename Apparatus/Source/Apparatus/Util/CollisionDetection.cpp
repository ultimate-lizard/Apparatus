#include "CollisionDetection.h"

#include <algorithm>

#include "../Rendering/Mesh.h"
#include "../Util/DebugPrimitive.h"
#include "../Core/Logger.h"

// TODO: Return bool and do parameter returns via pointers
std::pair<glm::vec3, glm::vec3> rayVsAABB(const glm::vec3& position, const glm::vec3& direction, const Box& box)
{
	glm::vec3 near = (box.position - (box.size / 2.0f) - position) / direction;
	glm::vec3 far = (box.position + (box.size / 2.0f) - position) / direction;

	if (near.x > far.x) std::swap(near.x, far.x);
	if (near.y > far.y) std::swap(near.y, far.y);
	if (near.z > far.z) std::swap(near.z, far.z);

	if (near.x > far.y || near.x > far.z ||
		near.y > far.x || near.y > far.z ||
		near.z > far.x || near.z > far.y)
	{
		return {};
	}

	float nearT = std::max({ near.x, near.y, near.z });
	float farT = std::min({ far.x, far.y, far.z });

	if (farT < 0.0f) return {};

	return { position + (direction * nearT), position + (direction * farT) };
}

std::vector<std::pair<glm::vec3, glm::vec3>> rayVsMesh(const glm::vec3& origin, const glm::vec3& direction, float length, const Mesh* mesh, const glm::mat4& meshTransform)
{
	std::vector<std::pair<glm::vec3, glm::vec3>> result;

	if (!mesh)
	{
		return result;
	}

	const std::vector<Vertex>& vertices = mesh->getVertices();
	const std::vector<unsigned int>& indices = mesh->getIndices();

	std::vector <glm::vec3> intersections;

	for (size_t i = 0; i < indices.size() - 3 && i < vertices.size() - 3; i += 3)
	{
		const Vertex& a = vertices[indices[i]];
		const Vertex& b = vertices[indices[i + 1]];
		const Vertex& c = vertices[indices[i + 2]];

		const glm::vec3 aNewPosition = a.position + glm::vec3(meshTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		const glm::vec3 bNewPosition = b.position + glm::vec3(meshTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		const glm::vec3 cNewPosition = c.position + glm::vec3(meshTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		glm::vec3 intersection = rayVsPolygon(origin, direction, aNewPosition, bNewPosition, cNewPosition);
		if (!glm::all(glm::isnan(intersection)))
		{
			// drawDebugPoint(intersection, { 1.0f, 1.0f, 0.0f, 1.0f }, 25.0f, true);
			intersections.push_back(intersection);
		}
	}

	for (unsigned int i = 0; i < intersections.size(); ++i)
	{
		if (i + 1 < intersections.size())
		{
			result.push_back({ intersections[i], intersections[i + 1] });
			++i;
		}
		else
		{
			result.push_back({ intersections[i], glm::vec3(std::numeric_limits<float>::quiet_NaN()) });
		}
	}

	return result;
}

glm::vec3 rayVsPolygon(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	glm::vec3 edgeA = b - a;
	glm::vec3 edgeB = c - a;

	const glm::vec3 pvec = glm::cross(direction, edgeB);
	float determinant = glm::dot(edgeA, pvec);

	const float EPSILON = 0.000001f;
	if (determinant > -EPSILON && determinant < EPSILON)
	{
		return glm::vec3(std::numeric_limits<float>::quiet_NaN());
	}

	// Inverting determinant to avoid multiple float divisionsq
	determinant = 1.0f / determinant;

	const glm::vec3 originToA = origin - a;

	float baryU = glm::dot(originToA, pvec) * determinant;
	if (baryU < 0.0f || baryU > 1.0f)
	{
		return glm::vec3(std::numeric_limits<float>::quiet_NaN());
	}

	const glm::vec3 crossOAvsEdge1 = glm::cross(originToA, edgeA);

	float baryV = glm::dot(direction, crossOAvsEdge1) * determinant;
	if (baryV < 0.0f || baryU + baryV > 1.0f)
	{
		return glm::vec3(std::numeric_limits<float>::quiet_NaN());
	}

	const float intersect = glm::dot(edgeB, crossOAvsEdge1) * determinant;

	if (intersect < EPSILON)
	{
		return glm::vec3(std::numeric_limits<float>::quiet_NaN());
	}
	
	return glm::vec3(origin + (direction * intersect));
}
