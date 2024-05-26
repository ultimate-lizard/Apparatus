#pragma once

#include <string>

class Entity;
class Client;

class Controller
{
public:
	Controller(const std::string& controllerName);
	virtual ~Controller() = default;

	Controller(const Controller&) = delete;
	Controller(Controller&&) = delete;
	void operator=(Controller&) = delete;

	virtual void setControlEntity(Entity* entity);
	virtual void setupInput() = 0;

	virtual void onActivate() = 0;
	virtual void onDeactivate() = 0;

	std::string getControllerName() const;

protected:
	Entity* controlEntity;
	std::string controllerName;
};
