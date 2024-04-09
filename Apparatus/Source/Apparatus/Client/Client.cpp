#include "Client.h"

#include "../Server/Entity.h"

Client::Client() :
	stopping(false),
	activeEntity(nullptr)
{
}

void Client::stop()
{
	stopping = true;
}

bool Client::isStopping() const
{
	return stopping;
}

void Client::setActiveEntity(Entity* entity)
{
	activeEntity = entity;

	onActiveEntitySet();
}

Entity* Client::getActiveEntity()
{
	return activeEntity;
}
