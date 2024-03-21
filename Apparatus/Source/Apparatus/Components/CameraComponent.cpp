#include "CameraComponent.h"

#include "../Server/Entity.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent(const std::string& id) :
	Component(id)
{

}

void CameraComponent::init()
{

}

void CameraComponent::update(float dt)
{
}

Camera& CameraComponent::getCamera()
{
	return camera;
}
