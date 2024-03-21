#include "BoundingBox.h"

#include <glm/gtc/quaternion.hpp>

#include "../Rendering/Model.h"
#include "../Server/LocalServer.h"

Box generateAABB(const Model* model, const glm::vec3& position, const glm::quat& orientation)
{
	if (!model)
	{
		return {};
	}

	std::vector<Vertex> modelVertices;

	const std::vector<Mesh*>& meshes = model->getMeshes();
	for (const Mesh* mesh : meshes)
	{
		if (!mesh)
		{
			continue;
		}

		const std::vector<Vertex>& meshVertices = mesh->getVertices();
		modelVertices.insert(modelVertices.end(), meshVertices.begin(), meshVertices.end());
	}

	glm::vec3 min(0.0f);
	glm::vec3 max(0.0f);

	Box newBox;

	for (const Vertex& vertex : modelVertices)
	{
		glm::vec3 rotatedPosition = (glm::mat4_cast(orientation) * glm::vec4(vertex.position, 1.0f));
		// glm::vec3 rotatedPosition = vertex.position;

		for (int i = 0; i < 3; ++i)
		{
			if (rotatedPosition[i] < min[i])
			{
				min[i] = rotatedPosition[i];
			}

			if (rotatedPosition[i] > max[i])
			{
				max[i] = rotatedPosition[i];
			}
		}
	}

	newBox.size = max - min;
	newBox.position = (min + max) / 2.0f;
	newBox.position += position;

	return newBox;
}

std::pair<glm::vec3, glm::vec3> rayVsAABB(const glm::vec3& position, const glm::vec3& direction, const Box& box)
{
	//float nearX = ((box.position.x - (box.size.x / 2.0f)) - position.x) / direction.x;
	//float nearY = ((box.position.y - (box.size.y / 2.0f)) - position.y) / direction.y;
	//float nearZ = ((box.position.z - (box.size.z / 2.0f)) - position.z) / direction.z;

	//float farX = ((box.position.x + (box.size.x / 2.0f)) - position.x) / direction.x;
	//float farY = ((box.position.y + (box.size.y / 2.0f)) - position.y) / direction.y;
	//float farZ = ((box.position.z + (box.size.z / 2.0f)) - position.z) / direction.z;

	//glm::vec3 near(nearX, nearY, nearZ);
	//glm::vec3 far(farX, farY, farZ);

	glm::vec3 near = (box.position - (box.size / 2.0f) - position) / direction;
	glm::vec3 far = (box.position + (box.size / 2.0f) - position) / direction;

	if (near.x > far.x) std::swap(near.x, far.x);
	if (near.y > far.y) std::swap(near.y, far.y);
	if (near.z > far.z) std::swap(near.z, far.z);

	// if (near.x > far.y || near.y > far.x) return {};
	// if (near.x > far.y || near.y > far.x || near.z > far.y || near.y > far.z || near.x > far.z || near.z > far.x) return {};
	if (near.x > far.y || near.x > far.z || near.y > far.x || near.y > far.z || near.z > far.x || near.z > far.y) return {};

	float nearT = std::max({ near.x, near.y, near.z });
	float farT = std::min({ far.x, far.y, far.z });

	if (farT < 0.0f) return {};

	// LocalServer::drawLine({ position, position + direction }, glm::vec4(1.0f), 5.0f, true);
	//LocalServer::drawPoint(position + (direction * nearT), {0.0f, 1.0f, 0.0f, 1.0f}, 10.0f, false, 3.0f);
	//LocalServer::drawPoint(position + (direction * farT), {1.0f, 0.0f, 0.0f, 1.0f}, 10.0f, false, 3.0f);

	return { position + (direction * nearT), position + (direction * farT) };
}
