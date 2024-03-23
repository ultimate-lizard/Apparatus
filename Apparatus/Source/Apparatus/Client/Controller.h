#pragma once

class Entity;
class Client;

#include "../Core/NameProvider.h"

class Controller : public NameProvider
{
public:
	Controller();
	Controller(const std::string& name);

	virtual ~Controller() = default;

	Controller(const Controller&) = delete;
	Controller(Controller&&) = delete;
	void operator=(Controller&) = delete;

	virtual void setControlEntity(Entity* entity);
	virtual void setupInput() {};

	virtual void onActivate() = 0;
	virtual void onDeactivate() = 0;

protected:
	Entity* controlledEntity;
};
