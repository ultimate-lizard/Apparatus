#include "CameraComponent.h"

#include "../Server/Entity.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent(Entity* owner) :
	Component(owner)
{
	assignDefaultObjectName();
}

Camera& CameraComponent::getCamera()
{
	return camera;
}

void CameraComponent::assignDefaultObjectName()
{
	if (owner)
	{
		setObjectName(owner->getObjectName() + "_CameraComponent");
	}
}
