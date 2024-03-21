#pragma once

#include "Controller.h"

class InputHandler;
class LocalClient;
class MovementComponent;

class HumanControllerBase : public Controller
{
public:
	HumanControllerBase(LocalClient* localClient);

	virtual void init() override;
	virtual void setupInput() = 0;
	virtual void setControlEntity(Entity* entity) override;

protected:
	LocalClient* localClient;
	InputHandler* inputHandler;
	MovementComponent* movementComponent;
};
