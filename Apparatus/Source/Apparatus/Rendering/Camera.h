#pragma once

#include <glm/glm.hpp>

#include "SceneNode.h"

class Camera : public SceneNode
{
public:
	Camera();
	~Camera() = default;

	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	void operator=(const Camera&) = delete;

	virtual void calculateTransform() override;

	void setFOV(float fov);
	void setAspect(float aspect);

	const glm::mat4& getView() const;
	const glm::mat4& getProjection() const;

private:
	glm::mat4 view;
	glm::mat4 projection;

	float aspect;
	float fov;
};
