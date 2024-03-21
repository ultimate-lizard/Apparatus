#pragma once

class Entity;
class Client;

class Controller
{
public:
	Controller();
	virtual ~Controller() = default;

	Controller(const Controller&) = delete;
	Controller(Controller&&) = delete;
	void operator=(Controller&) = delete;

	virtual void init() = 0;
	virtual void setControlEntity(Entity* entity);
	virtual void setupInput() {};

	virtual void onActivate() = 0;

protected:
	Entity* controlledEntity;
};
