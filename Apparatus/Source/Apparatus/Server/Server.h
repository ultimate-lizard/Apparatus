#pragma once

#include "../Core/LifecycleInterface.h"
#include "../Core/NameProvider.h"

class Apparatus;

class Server : public LifecycleInterface, public NameProvider
{
public:
	Server(Apparatus* apparatus);
	virtual ~Server() = default;

	Server(const Server&) = delete;
	Server(Server&&) = delete;
	void operator=(const Server&) = delete;

	virtual void update(float dt) = 0;
	virtual void shutdown();

	bool isShuttingDown() const;

protected:
	Apparatus* apparatus;
	bool shuttingDown;
};
