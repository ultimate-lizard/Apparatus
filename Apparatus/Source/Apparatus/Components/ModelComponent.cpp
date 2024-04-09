#include "ModelComponent.h"

#include "TransformComponent.h"
#include "../Server/Entity.h"
#include "../Rendering/Model.h"
#include "../Rendering/Material.h"

ModelComponent::ModelComponent() :
	Component("ModelComponent"),
	model(nullptr),
	visible(true),
	depthBufferLayer(0)
{

}

ModelComponent::ModelComponent(const std::string& componentName) :
	Component(componentName),
	model(nullptr),
	visible(true),
	depthBufferLayer(0)
{
}

ModelComponent::ModelComponent(const ModelComponent& other) :
	Component(other),
	SceneNode(other),
	model(nullptr),
	visible(other.visible),
	depthBufferLayer(other.depthBufferLayer)
{
	setModel(other.model);
}

std::unique_ptr<Component> ModelComponent::clone()
{
	return std::make_unique<ModelComponent>(*this);
}

void ModelComponent::init()
{
	Component::init();
}

void ModelComponent::setModel(Model* model)
{
	if (model)
	{
		this->model = model;

		modelInstance = this->model->createModelInstance();

		if (modelInstance)
		{
			modelInstance->setDepthBufferLayer(depthBufferLayer);
		}
	}
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
