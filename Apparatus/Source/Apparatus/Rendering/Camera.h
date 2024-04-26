#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(const Camera& other);
	~Camera() = default;

	Camera(Camera&&) = delete;

	void calculateView();
	
	void setPerspective(float fov, float aspect, float near, float far);
	void setOrthographic(float left, float right, float top, float bottom, float near, float far);

	const glm::vec3& getPosition() const;
	const glm::vec3& getForward() const;
	const glm::vec3& getUp() const;

	void setFOV(float fov);
	void setAspect(float aspect);

	const glm::mat4& getView() const;
	const glm::mat4& getProjection() const;

private:
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

	float fov;
	float aspect;
	float near;
	float far;

	friend class CameraComponent;
};
