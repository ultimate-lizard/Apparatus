#include "HumanPlayerController.h"

#include <functional>

#include <SDL2/SDL.h>

#include "InputHandler.h"
#include "LocalClient.h"
#include "../Core/Logger.h"
#include "../Components/DebugMovementComponent.h"
#include "../Server/Entity.h"
#include "../Apparatus.h"

HumanPlayerController::HumanPlayerController(LocalClient* localClient) :
	HumanControllerBase(localClient)
{
}

void HumanPlayerController::onActivate()
{
	if (localClient)
	{
		if (Apparatus* apparatus = localClient->getApparatus())
		{
			apparatus->setCursorVisibleEnabled(false);
		}
	}
}

void HumanPlayerController::moveZ(float rate)
{
	if (movementComponent)
	{
		movementComponent->moveZ(rate);
	}
}

void HumanPlayerController::moveX(float rate)
{
	if (movementComponent)
	{
		movementComponent->moveX(rate);
	}
}

void HumanPlayerController::lookX(float value)
{
	if (movementComponent)
	{
		movementComponent->lookX(value);
	}
}

void HumanPlayerController::lookY(float value)
{
	if (movementComponent)
	{
		movementComponent->lookY(value);
	}
}

void HumanPlayerController::toggleDebugMode()
{
	if (localClient)
	{
		localClient->setDebugModeEnabled(true);
	}

	if (movementComponent)
	{
		movementComponent->lookX(0.0f);
		movementComponent->lookY(0.0f);
		movementComponent->moveX(0.0f);
		movementComponent->moveZ(0.0f);
	}

	LOG("Edit mode", LogLevel::Info);
}

void HumanPlayerController::setupInput()
{
	if (!inputHandler)
	{
		return;
	}

	inputHandler->bindAxisAction("LookX", std::bind(&HumanPlayerController::lookX, this, std::placeholders::_1));
	inputHandler->bindAxisAction("LookY", std::bind(&HumanPlayerController::lookY, this, std::placeholders::_1));

	inputHandler->bindAxisAction("MoveZ", std::bind(&HumanPlayerController::moveZ, this, std::placeholders::_1));
	inputHandler->bindAxisAction("MoveX", std::bind(&HumanPlayerController::moveX, this, std::placeholders::_1));

	inputHandler->bindKeyAction("ToggleDebugMode", KeyEventType::Press, std::bind(&HumanPlayerController::toggleDebugMode, this));
	inputHandler->bindKeyAction("ToggleDebugMode", KeyEventType::Press, std::bind(&HumanPlayerController::toggleDebugMode, this));
}
