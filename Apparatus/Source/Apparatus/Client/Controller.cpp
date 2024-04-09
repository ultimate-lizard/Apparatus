#include "Controller.h"

Controller::Controller(const std::string& controllerName) :
	controllerName(controllerName),
	controlEntity(nullptr)
{

}

void Controller::setControlEntity(Entity* entity)
{
	controlEntity = entity;
}

std::string Controller::getControllerName() const
{
	return controllerName;
}
