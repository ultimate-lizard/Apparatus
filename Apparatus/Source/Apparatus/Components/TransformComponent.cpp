#include "TransformComponent.h"

#include "../Rendering/Model.h"
#include "../Server/LocalServer.h"

TransformComponent::TransformComponent(const std::string& id) :
	Component(id),
	debugModel(nullptr)
{

}

void TransformComponent::init()
{
	if (debugModel)
	{
		debugModelInstance = debugModel->createModelInstance();
	}
}

void TransformComponent::update(float dt)
{
	calculateTransform();

	glm::vec3 forward = getForward();
	glm::vec3 right = getRight();
	glm::vec3 up = getUp();
	glm::vec3 position = getDerivedPosition();

	LocalServer::drawLine({ position, position + right * 3.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 2.0f);
	LocalServer::drawLine({ position, position + up * 3.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 2.0f);
	LocalServer::drawLine({ position, position + forward * 3.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, 2.0f);
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
