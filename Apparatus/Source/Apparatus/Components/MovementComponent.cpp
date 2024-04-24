#include "MovementComponent.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "../Server/Entity.h"
#include "../Server/LocalServer.h"
#include "../Core/Logger.h"

MovementComponent::MovementComponent() :
	Component("MovementComponent"),
	forward(0),
	sideways(0)
{
}

MovementComponent::MovementComponent(const std::string& componentName) :
	Component(componentName),
	forward(0),
	sideways(0)
{
}

MovementComponent::MovementComponent(const MovementComponent& other) :
	Component(other),
	forward(other.forward),
	sideways(other.sideways)
{
	
}

std::unique_ptr<Component> MovementComponent::clone()
{
	return std::make_unique<MovementComponent>(*this);
}

void MovementComponent::init()
{
	Component::init();

	if (!owner)
	{
		return;
	}

	if (TransformComponent* transformComponent = owner->findComponent<TransformComponent>())
	{
		headRotator = transformComponent->getRotator();
	}
}

void MovementComponent::update(float dt)
{
	if (!owner)
	{
		return;
	}

	if (TransformComponent* transformComponent = owner->findComponent<TransformComponent>())
	{
		if (CameraComponent* cameraComponent = owner->findComponent<CameraComponent>())
		{
			Camera& camera = cameraComponent->getCamera();

			const float moveSpeed = 10.0f;

			glm::vec3 movementVector(0.0f);

			if (forward)
			{
				glm::vec3 front = transformComponent->getLocalForward();

				float direction = forward > 0 ? 1.0f : -1.0f;
				movementVector += front * direction;
			}

			if (sideways)
			{
				glm::vec3 right = transformComponent->getLocalRight();

				float direction = sideways > 0 ? -1.0f : 1.0f;
				movementVector += right * direction;
			}

			transformComponent->setRotation(headRotator.get(Euler::Pitch), Euler::Pitch);
			transformComponent->setRotation(headRotator.get(Euler::Yaw), Euler::Yaw);
			transformComponent->offsetPosition(movementVector * dt * moveSpeed);
		}
	}
}

void MovementComponent::moveZ(float rate)
{
	forward = static_cast<int>(rate);
}

void MovementComponent::moveX(float rate)
{
	sideways = static_cast<int>(-rate);
}

void MovementComponent::lookX(float rate)
{
	const float sensitivity = 0.1f;
	headRotator.rotate(-rate * sensitivity, Euler::Yaw);
}

void MovementComponent::lookY(float rate)
{
	const float sensitivity = 0.1f;

	headRotator.rotate(rate * sensitivity, Euler::Pitch);
	headRotator.clamp(90.0f, 270.0f, Euler::Pitch);
}
