#include "Component.h"

#include "../Core/Logger.h"

Component::Component(const std::string& id) :
	id(id),
	owner(nullptr)
{
	LOG("Created component " + this->id, LogLevel::Trace);
}

void Component::setId(const std::string& id)
{
	this->id = id;
}

std::string Component::getId() const
{
	return id;
}

void Component::setOwner(Entity* owner)
{
	this->owner = owner;
}

Entity* Component::getOwner()
{
	return owner;
}
