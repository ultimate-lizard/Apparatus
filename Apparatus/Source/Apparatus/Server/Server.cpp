#include "Server.h"

Server::Server(Apparatus* apparatus) :
	apparatus(apparatus),
	shuttingDown(false)
{
}

void Server::shutdown()
{
	shuttingDown = true;
}

bool Server::isShuttingDown() const
{
	return shuttingDown;
}
