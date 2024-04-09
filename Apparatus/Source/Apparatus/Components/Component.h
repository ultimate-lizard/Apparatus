#pragma once

#include <string>
#include <memory>

#include "../Core/LifecycleInterface.h"
#include "../Core/NameProvider.h"

class Entity;

class Component
{
public:
	Component(const std::string& name);
	Component(const Component& other);
	virtual ~Component() = default;

	Component(Component&&) = delete;

	virtual std::unique_ptr<Component> clone() = 0;

	virtual void init();
	virtual void onEntitySpawn() {};
	virtual void update(float dt) {};

	void setOwner(Entity* owner);
	Entity* getOwner();

	void setComponentName(const std::string& componentName);
	const std::string& getComponentName() const;

	bool operator<(const Component& other)
	{
		return other.componentName != componentName;
	}

protected:
	Entity* owner;

	std::string componentName;
};
