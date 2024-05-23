#include "BoundingBox.h"

#include <glm/gtc/quaternion.hpp>

#include "../Rendering/Model.h"
#include "../Server/LocalServer.h"
#include "../Util/DebugPrimitive.h"
#include "../Core/Logger.h"
#include "../Rendering/Mesh.h"

Box generateAABB(const Model* model, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
{
	if (!model)
	{
		return {};
	}

	std::vector<ModelVertex> modelVertices;

	const std::vector<Mesh*>& meshes = model->getMeshes();
	for (const Mesh* mesh : meshes)
	{
		if (!mesh)
		{
			continue;
		}

		if (std::shared_ptr<VertexBuffer<ModelVertex>> meshVertices = mesh->getVertexBuffer<VertexBuffer<ModelVertex>>())
		{
			modelVertices.insert(modelVertices.end(), meshVertices->vertices.begin(), meshVertices->vertices.end());
		}
		else
		{
			assert(false);
		}
	}

	glm::vec3 min(0.0f);
	glm::vec3 max(0.0f);

	Box newBox;

	for (const ModelVertex& vertex : modelVertices)
	{
		glm::vec3 scaledPosition = vertex.position * scale;
		glm::vec3 rotatedPosition = (glm::mat4_cast(orientation) * glm::vec4(scaledPosition, 1.0f));

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

Box generateAABB(const std::vector<Model*>& models, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
{
	std::vector<ModelVertex> modelVertices;

	for (Model* model : models)
	{
		const std::vector<Mesh*>& meshes = model->getMeshes();
		for (const Mesh* mesh : meshes)
		{
			if (!mesh)
			{
				continue;
			}

			if (std::shared_ptr<VertexBuffer<ModelVertex>> meshVertices = mesh->getVertexBuffer<VertexBuffer<ModelVertex>>())
			{
				modelVertices.insert(modelVertices.end(), meshVertices->vertices.begin(), meshVertices->vertices.end());
			}
			else
			{
				assert(false);
			}
		}
	}

	glm::vec3 min(0.0f);
	glm::vec3 max(0.0f);

	Box newBox;

	for (const ModelVertex& vertex : modelVertices)
	{
		glm::vec3 scaledPosition = vertex.position * scale;
		glm::vec3 rotatedPosition = (glm::mat4_cast(orientation) * glm::vec4(scaledPosition, 1.0f));

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
