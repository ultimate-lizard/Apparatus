#include "ModelComponent.h"

#include "TransformComponent.h"
#include "../Server/Entity.h"
#include "../Rendering/Model.h"
#include "../Rendering/Material.h"

ModelComponent::ModelComponent(const std::string& id) :
	Component(id),
	model(nullptr)
{
}

void ModelComponent::init()
{
	if (model)
	{
		modelInstance = std::move(model->createModelInstance());
	}
}

void ModelComponent::setModel(Model* model)
{
	this->model = model;
}

Model* ModelComponent::getModel()
{
	return model;
}

const Model* ModelComponent::getModel() const
{
	return model;
}

ModelInstance* ModelComponent::getModelInstance()
{
	return modelInstance.get();
}

