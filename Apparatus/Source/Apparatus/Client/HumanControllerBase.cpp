#include "HumanControllerBase.h"

#include "LocalClient.h"
#include "../Server/Entity.h"
#include "../Components/MovementComponent.h"

HumanControllerBase::HumanControllerBase(const std::string& controllerName, LocalClient& localClient) :
	Controller(controllerName),
	localClient(localClient),
	movementComponent(nullptr)
{

}

void HumanControllerBase::setControlEntity(Entity* entity)
{
	Controller::setControlEntity(entity);

	if (controlEntity)
	{
		movementComponent = controlEntity->findComponent<MovementComponent>();
	}
	else
	{
		movementComponent = nullptr;
	}
}
