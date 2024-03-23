#include "GenericHumanController.h"

#include <functional>

#include <SDL2/SDL.h>

#include "InputHandler.h"
#include "LocalClient.h"
#include "../Core/Logger.h"
#include "../Components/DebugMovementComponent.h"
#include "../Server/Entity.h"
#include "../Apparatus.h"

GenericHumanController::GenericHumanController(LocalClient* localClient) :
	HumanControllerBase(localClient)
{
	assignDefaultObjectName();
}

void GenericHumanController::assignDefaultObjectName()
{
	setObjectName("GenericHumanController");
}

void GenericHumanController::onActivate()
{
	HumanControllerBase::onActivate();

	if (localClient)
	{
		if (Apparatus* apparatus = localClient->getApparatus())
		{
			apparatus->setCursorVisibleEnabled(false);
		}
	}
}

void GenericHumanController::onDeactivate()
{
	if (controlledEntity)
	{
		if (auto movement = controlledEntity->findComponent<MovementComponent>())
		{
			movement->moveX(0.0f);
			movement->moveZ(0.0f);
		}
	}
}

void GenericHumanController::moveZ(float rate)
{
	if (movementComponent)
	{
		movementComponent->moveZ(rate);
	}
}

void GenericHumanController::moveX(float rate)
{
	if (movementComponent)
	{
		movementComponent->moveX(rate);
	}
}

void GenericHumanController::lookX(float value)
{
	if (movementComponent)
	{
		movementComponent->lookX(value);
	}
}

void GenericHumanController::lookY(float value)
{
	if (movementComponent)
	{
		movementComponent->lookY(value);
	}
}

void GenericHumanController::setupInput()
{
	if (!inputHandler)
	{
		return;
	}

	inputHandler->bindAxisAction("LookX", std::bind(&GenericHumanController::lookX, this, std::placeholders::_1));
	inputHandler->bindAxisAction("LookY", std::bind(&GenericHumanController::lookY, this, std::placeholders::_1));

	inputHandler->bindAxisAction("MoveZ", std::bind(&GenericHumanController::moveZ, this, std::placeholders::_1));
	inputHandler->bindAxisAction("MoveX", std::bind(&GenericHumanController::moveX, this, std::placeholders::_1));
}
