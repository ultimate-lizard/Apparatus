#include "HumanControllerBase.h"

#include "LocalClient.h"
#include "../Server/Entity.h"
#include "../Components/MovementComponent.h"

HumanControllerBase::HumanControllerBase(LocalClient* localClient) :
	localClient(localClient),
	inputHandler(nullptr),
	movementComponent(nullptr)
{

}

void HumanControllerBase::onActivate()
{
	if (localClient)
	{
		inputHandler = &localClient->getInputHandler();
	}
}

void HumanControllerBase::setControlEntity(Entity* entity)
{
	Controller::setControlEntity(entity);

	if (controlledEntity)
	{
		movementComponent = controlledEntity->findComponent<MovementComponent>();
	}
}
