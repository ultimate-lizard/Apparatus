#pragma once

class Apparatus;

class Server
{
public:
	Server(Apparatus* apparatus);
	virtual ~Server() = default;

	Server(const Server&) = delete;
	Server(Server&&) = delete;
	void operator=(const Server&) = delete;

	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void shutdown();

	bool isShuttingDown() const;

protected:
	Apparatus* apparatus;
	bool shuttingDown;
};
