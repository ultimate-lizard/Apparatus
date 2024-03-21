#include "Controller.h"

Controller::Controller() :
	controlledEntity(nullptr)
{

}

void Controller::setControlEntity(Entity* entity)
{
	controlledEntity = entity;
}
