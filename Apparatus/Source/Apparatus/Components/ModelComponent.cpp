#include "ModelComponent.h"

#include "TransformComponent.h"
#include "../Server/Entity.h"
#include "../Rendering/Model.h"
#include "../Rendering/Material.h"

ModelComponent::ModelComponent(Entity* owner) :
	Component(owner),
	model(nullptr),
	visible(true),
	depthBufferLayer(0)
{
	assignDefaultObjectName();
}

ModelComponent::ModelComponent(Entity* owner, const std::string& name) :
	Component(owner, name),
	model(nullptr),
	visible(true),
	depthBufferLayer(0)
{
	
}

void ModelComponent::init()
{
	Component::init();

	if (model)
	{
		modelInstance = std::move(model->createModelInstance());

		if (modelInstance)
		{
			modelInstance->setDepthBufferLayer(depthBufferLayer);
		}
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

void ModelComponent::setVisibility(bool enabled)
{
	this->visible = enabled;
}

bool ModelComponent::isVisible() const
{
	return visible == true;
}

void ModelComponent::setDepthBufferLayer(size_t layer)
{
	if (modelInstance)
	{
		modelInstance->setDepthBufferLayer(layer);
	}

	depthBufferLayer = layer;
}

size_t ModelComponent::getDepthBufferLayer() const
{
	if (modelInstance)
	{
		return modelInstance->getDepthBufferLayer();
	}
	
	return depthBufferLayer;
}

void ModelComponent::assignDefaultObjectName()
{
	if (owner)
	{
		setObjectName(owner->getObjectName() + "_ModelComponent");
	}
}

