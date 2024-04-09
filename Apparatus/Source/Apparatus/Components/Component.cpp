#include "Component.h"

#include "../Core/Logger.h"

Component::Component(const std::string& componentName) :
	owner(nullptr),
	componentName(componentName)
{
}

Component::Component(const Component& other) :
	owner(other.owner),
	componentName(other.componentName)
{
}

void Component::init()
{
	LOG("Initialized component '" + componentName + "'", LogLevel::Info);
}

void Component::setOwner(Entity* owner)
{
	this->owner = owner;
}

Entity* Component::getOwner()
{
	return owner;
}

void Component::setComponentName(const std::string& componentName)
{
	this->componentName = componentName;
}

const std::string& Component::getComponentName() const
{
	return componentName;
}
