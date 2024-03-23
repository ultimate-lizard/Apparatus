#include "Entity.h"

#include "../Core/Logger.h"

Entity::Entity(const std::string& name) :
	NameProvider(name)
{
}

void Entity::init()
{
	LOG("Initialized entity '" + getObjectName() + "'", LogLevel::Info);
}

void Entity::addComponent(Component* component)
{
	components.push_back(component);
}

void Entity::assignDefaultObjectName()
{
	setObjectName("UnnamedEntity");
}
