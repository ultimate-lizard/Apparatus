#include "Server.h"

Server::Server() :
	stopping(false)
{
}

void Server::stop()
{
	stopping = true;
}

bool Server::isStopping() const
{
	return stopping;
}
