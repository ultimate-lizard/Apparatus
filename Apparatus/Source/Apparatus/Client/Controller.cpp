#include "Controller.h"

Controller::Controller() :
	controlledEntity(nullptr)
{

}

Controller::Controller(const std::string& name) :
	NameProvider(name),
	controlledEntity(nullptr)
{

}

void Controller::setControlEntity(Entity* entity)
{
	controlledEntity = entity;
}
