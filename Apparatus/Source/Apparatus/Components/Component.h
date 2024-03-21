#pragma once

#include <string>

class Entity;

class Component
{
public:
	Component(const std::string& id);
	virtual ~Component() = default;

	Component(const Component&) = delete;
	Component(Component&&) = delete;
	void operator=(const Component&) = delete;

	void setId(const std::string& id);
	std::string getId() const;

	void setOwner(Entity* owner);
	Entity* getOwner();

	virtual void init() = 0;
	virtual void update(float dt) = 0;

protected:
	std::string id;

	Entity* owner;
};
