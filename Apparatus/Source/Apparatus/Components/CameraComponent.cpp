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

std::unique_ptr<Component> CameraComponent::clone()
{
	std::unique_ptr<CameraComponent> newCameraComponent = std::make_unique<CameraComponent>(*this);
	if (newCameraComponent)
	{
		Camera& newCamera = newCameraComponent->getCamera();
		newCamera.setParent(newCameraComponent.get());
	}

	return newCameraComponent;
}

void CameraComponent::init()
{
	Component::init();

	camera.setParent(this);
}

Camera& CameraComponent::getCamera()
{
	return camera;
}
