#include "DebugMovementComponent.h"

#include <glm/glm.hpp>

#include "../Server/Entity.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "../Core/Logger.h"

DebugCameraMovementComponent::DebugCameraMovementComponent() :
	MovementComponent("DebugCameraMovementComponent"),
	roll(0)
{
	
}

DebugCameraMovementComponent::DebugCameraMovementComponent(const std::string& componentName) :
	MovementComponent(componentName),
	roll(0)
{
}

void DebugCameraMovementComponent::update(float dt)
{
	MovementComponent::update(dt);

	if (roll)
	{
		headRotator.rotate(static_cast<float>(roll), Euler::Roll);
	}
}

void DebugCameraMovementComponent::rotateRoll(float rate)
{
	roll = static_cast<int>(rate);
}
