#include "CollisionDetection.h"

#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Apparatus.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Vector.h"
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

std::vector<std::pair<glm::vec3, glm::vec3>> rayVsMesh(const glm::vec3& origin, const glm::vec3& direction, const Mesh* mesh, const glm::mat4& meshTransform)
{
	std::vector<std::pair<glm::vec3, glm::vec3>> result;

	if (!mesh)
	{
		return result;
	}

	const std::vector<Vertex>& vertices = mesh->getVertices();
	const std::vector<unsigned int>& indices = mesh->getIndices();

	std::vector <glm::vec3> intersections;

	for (size_t i = 0; i < indices.size(); i += 3)
	{
		const Vertex& a = vertices[indices[i]];
		const Vertex& b = vertices[indices[i + 1]];
		const Vertex& c = vertices[indices[i + 2]];

		const glm::vec4 aNewPosition = meshTransform * glm::vec4(a.position, 1.0f);
		const glm::vec4 bNewPosition = meshTransform * glm::vec4(b.position, 1.0f);
		const glm::vec4 cNewPosition = meshTransform * glm::vec4(c.position, 1.0f);

		const glm::vec3 normal = glm::normalize(meshTransform * glm::vec4(a.normal, 0.0f));

		glm::vec3 intersection = rayVsPolygon(origin, direction, aNewPosition, bNewPosition, cNewPosition, normal);
		if (!glm::any(glm::isnan(intersection)))
		{
			intersections.push_back(intersection);
		}
	}

	// Sort the nears and fars
	std::sort(intersections.begin(), intersections.end(), [&origin](glm::vec3& a, glm::vec3& b) {
		return glm::distance(a, origin) < glm::distance(b, origin);
	});

	bool near = true;
	for (unsigned int i = 0; i < intersections.size(); ++i)
	{
		if (i + 1 < intersections.size())
		{
			result.push_back({ intersections[i], intersections[i + 1] });
			i++;
		}
		else
		{
			result.push_back({ intersections[i], glm::vec3(std::numeric_limits<float>::quiet_NaN())});
		}
		
	}

	return result;
}

glm::vec3 rayVsPolygon(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& normal)
{
	glm::vec3 p = rayVsPlane((a + b + c) / 3.0f, normal, rayOrigin, rayDirection);

	if (glm::any(glm::isnan(p)))
	{
		return glm::vec3(std::numeric_limits<float>::quiet_NaN());
	}

	glm::vec3 v0 = b - a;
	glm::vec3 v1 = c - a;
	glm::vec3 v2 = p - a;

	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);

	float denom = d00 * d11 - d01 * d01;
	float barA = (d11 * d20 - d01 * d21) / denom;
	float barB = (d00 * d21 - d01 * d20) / denom;
	float barC = 1.0f - barA - barB;

	if (barA >= 0.0f && barA < 1.0f && barB >= 0.0f && barB < 1.0f && barC >= 0.0f && barC < 1.0f)
	{
		return p;
	}

	return glm::vec3(std::numeric_limits<float>::quiet_NaN());
}

glm::vec3 rayVsPlane(const glm::vec3& planeOrigin, const glm::vec3& planeNormal, const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
{
	if (float denominator = glm::dot(rayDirection, planeNormal))
	{
		if (glm::abs(denominator) > std::numeric_limits<float>::epsilon())
		{
			const float t = glm::dot((planeOrigin - rayOrigin), planeNormal) / denominator;
			if (t >= 0.0f && !glm::isinf(t))
			{
				return rayOrigin + (rayDirection * t);
			}
		}
	}
	
	return glm::vec3(std::numeric_limits<float>::quiet_NaN());
}

glm::vec2 getCursorToDevice()
{
	Apparatus& app = Apparatus::get();
	const Window& window = app.getWindow();

	const glm::ivec2& mousePos = window.getMouseCursorPosition();
	const glm::vec2& windowSize = window.getWindowSize();

	float normalX = (2.0f * mousePos.x) / windowSize.x - 1.0f;
	float normalY = 1.0f - (2.0f * mousePos.y) / windowSize.y;

	return glm::vec2(normalX, normalY);
}

glm::vec3 getCursorToWorldRay(const glm::mat4& view, const glm::mat4& projection)
{
	glm::vec2 normal = getCursorToDevice();

	glm::vec4 rayClip(normal.x, normal.y, -1.0f, 1.0f);

	glm::vec4 rayEye = glm::inverse(projection) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	return glm::normalize(glm::inverse(view) * rayEye);
}
