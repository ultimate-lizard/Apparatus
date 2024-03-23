#include "Component.h"

#include "../Core/Logger.h"

Component::Component(Entity* owner) :
	owner(owner)
{
	
}

Component::Component(Entity* owner, const std::string& name) :
	NameProvider(name),
	owner(owner)
{
}

void Component::init()
{
	LOG("Initialized component '" + getObjectName() + "'", LogLevel::Info);
}

void Component::setOwner(Entity* owner)
{
	this->owner = owner;
}

Entity* Component::getOwner()
{
	return owner;
}
