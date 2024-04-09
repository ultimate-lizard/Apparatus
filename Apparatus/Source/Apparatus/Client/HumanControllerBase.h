#pragma once

#include "Controller.h"

class InputHandler;
class MovementComponent;
class LocalClient;

class HumanControllerBase : public Controller
{
public:
	HumanControllerBase(const std::string& controllerName, LocalClient& localClient);

	virtual void onActivate() = 0;
	virtual void setupInput() = 0;
	virtual void setControlEntity(Entity* entity) override;

protected:
	LocalClient& localClient;
	MovementComponent* movementComponent;
};
