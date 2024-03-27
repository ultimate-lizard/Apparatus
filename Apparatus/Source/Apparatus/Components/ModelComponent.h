#pragma once

#include <memory>

#include "Component.h"
#include "../Rendering/SceneNode.h"
#include "../Rendering/Model.h"

class ModelComponent : public Component, public SceneNode
{
public:
	ModelComponent(Entity* owner);
	ModelComponent(Entity* owner, const std::string& name);

	virtual void init() override;
	virtual void update(float dt) override {}

	void setModel(Model* model);
	Model* getModel();
	const Model* getModel() const;

	ModelInstance* getModelInstance();

	void setVisibility(bool enabled);
	bool isVisible() const;

protected:
	virtual void assignDefaultObjectName() override;

	Model* model;
	std::unique_ptr<ModelInstance> modelInstance;

	bool visible;
};
