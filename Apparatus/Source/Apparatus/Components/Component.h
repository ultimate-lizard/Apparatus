#pragma once

#include <string>

#include "../Core/LifecycleInterface.h"
#include "../Core/NameProvider.h"

class Entity;

class Component : public LifecycleInterface, public NameProvider
{
public:
	Component(Entity* owner);
	Component(Entity* owner, const std::string& name);
	virtual ~Component() = default;

	Component(const Component&) = delete;
	Component(Component&&) = delete;
	void operator=(const Component&) = delete;

	virtual void init() override;
	virtual void start() override {};
	virtual void update(float dt) override {};

	void setOwner(Entity* owner);
	Entity* getOwner();

protected:
	Entity* owner;
};
