#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Core/Logger.h"


Camera::Camera() :
	view(1.0f),
	projection(1.0f),
	aspect(800.0f / 600.0f),
	fov(90.0f)
{

}

void Camera::calculateTransform()
{
	SceneNode::calculateTransform();

	view = glm::lookAt(getWorldPosition(), getWorldPosition() + getForward(), getUp());
	projection = glm::perspective(glm::radians(fov), aspect, 0.01f, 1000.0f);
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
