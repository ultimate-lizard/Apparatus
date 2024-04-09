#pragma once

#include <memory>

#include "Component.h"
#include "../Rendering/SceneNode.h"
#include "../Rendering/Model.h"

class ModelComponent : public Component, public SceneNode
{
public:
	ModelComponent();
	ModelComponent(const std::string& componentName);
	ModelComponent(const ModelComponent& other);

	virtual std::unique_ptr<Component> clone() override;

	virtual void init() override;
	virtual void update(float dt) override {}

	void setModel(Model* model);
	Model* getModel();
	const Model* getModel() const;

	ModelInstance* getModelInstance();

	void setVisibility(bool enabled);
	bool isVisible() const;

	void setDepthBufferLayer(size_t layer);
	size_t getDepthBufferLayer() const;

protected:
	Model* model;
	std::unique_ptr<ModelInstance> modelInstance;

	bool visible;

	size_t depthBufferLayer;
};
