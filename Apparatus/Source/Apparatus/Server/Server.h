#pragma once

class Apparatus;
class Client;

struct ConnectionInfo
{
	Client* client;
};

class Server
{
public:
	Server();
	virtual ~Server() = default;

	Server(const Server&) = delete;
	Server(Server&&) = delete;
	void operator=(const Server&) = delete;

	virtual void init() = 0;
	virtual void start() = 0;
	virtual void update(float dt) = 0;
	virtual void stop();

	bool isStopping() const;

	virtual void connect(const ConnectionInfo& info) = 0;

protected:
	bool stopping;
};
