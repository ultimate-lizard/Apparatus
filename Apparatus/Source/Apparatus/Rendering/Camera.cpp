#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Core/Logger.h"

Camera::Camera() :
	view(1.0f),
	projection(1.0f),
	position(0.0f),
	forward(0.0f, 0.0f, 1.0f),
	up(0.0f, 1.0f, 1.0f),
	fov(90.0f),
	aspect(800.0f / 600.0f),
	near(0.01f),
	far(10000.0f)
{
}

Camera::Camera(const Camera& other) :
	view(other.view),
	projection(other.projection),
	position(other.position),
	forward(other.forward),
	up(other.up),
	fov(other.fov),
	aspect(other.aspect),
	near(other.near),
	far(other.near)
{
}

void Camera::calculateView()
{
	view = glm::lookAt(position, position + forward, up);
}

void Camera::setPerspective(float fov, float aspect, float near, float far)
{
	projection = glm::perspective(glm::radians(fov), aspect, near, far);
	this->fov = fov;
	this->aspect = aspect;
	this->near = near;
	this->far = far;

}

void Camera::setOrthographic(float left, float right, float top, float bottom, float, float)
{
	projection = glm::ortho(left, right, bottom, top, near, far);
}

const glm::vec3& Camera::getPosition() const
{
	return position;
}

const glm::vec3& Camera::getForward() const
{
	return forward;
}

const glm::vec3& Camera::getUp() const
{
	return up;
}

void Camera::setFOV(float fov)
{
	this->fov = fov;
}

void Camera::setAspect(float aspect)
{
	this->aspect = aspect;
}

const glm::mat4& Camera::getView() const
{
	return view;
}

const glm::mat4& Camera::getProjection() const
{
	return projection;
}
