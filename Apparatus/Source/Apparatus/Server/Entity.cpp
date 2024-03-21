#include "Entity.h"

#include "../Core/Logger.h"

Entity::Entity(const std::string& id) :
	id(id)
{
	LOG("Created entity " + this->id, LogLevel::Trace);
}

void Entity::setId(const std::string& id)
{
	this->id = id;
}

std::string Entity::getId() const
{
	return id;
}

void Entity::addComponent(Component* component)
{
	components.push_back(component);
}
