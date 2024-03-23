#include "TransformComponent.h"

#include "../Rendering/Model.h"
#include "../Server/LocalServer.h"

TransformComponent::TransformComponent(Entity* owner) :
	Component(owner),
	debugModel(nullptr)
{
	assignDefaultObjectName();
}

void TransformComponent::init()
{
	Component::init();

	if (debugModel)
	{
		debugModelInstance = debugModel->createModelInstance();
	}
}

void TransformComponent::update(float dt)
{
	calculateTransform();
}

void TransformComponent::setDebugModel(Model* model)
{
	debugModel = model;
}

Model* TransformComponent::getDebugModel()
{
	return debugModel;
}

ModelInstance* TransformComponent::getDebugModelInstance()
{
	return debugModelInstance.get();
}

void TransformComponent::assignDefaultObjectName()
{
	if (owner)
	{
		setObjectName(owner->getObjectName() + "_TransformComponent");
	}
}
