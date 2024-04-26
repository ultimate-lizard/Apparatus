#include "CameraComponent.h"

#include "../Server/Entity.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent() :
	Component("CameraComponent")
{

}

CameraComponent::CameraComponent(const std::string& componentName) :
	Component(componentName)
{
}

CameraComponent::CameraComponent(const CameraComponent& other) :
	Component(other),
	SceneNode(other),
	camera(other.camera)
{
}

void CameraComponent::calculateTransform()
{
	if (isDirty())
	{
		camera.position = getWorldPosition();
		camera.forward = getForward();
		camera.up = getUp();

		camera.calculateView();
	}

	SceneNode::calculateTransform();
}

std::unique_ptr<Component> CameraComponent::clone()
{
	return std::make_unique<CameraComponent>(*this);
}

void CameraComponent::init()
{
	Component::init();
}

Camera& CameraComponent::getCamera()
{
	return camera;
}
