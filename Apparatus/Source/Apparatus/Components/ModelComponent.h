#pragma once

#include <memory>

#include "Component.h"
#include "../Rendering/SceneNode.h"
#include "../Rendering/Model.h"

class ModelComponent : public Component, public SceneNode
{
public:
	ModelComponent(const std::string& id);

	virtual void init() override;
	virtual void update(float dt) override {}

	void setModel(Model* model);
	Model* getModel();
	const Model* getModel() const;

	ModelInstance* getModelInstance();

protected:
	Model* model;
	std::unique_ptr<ModelInstance> modelInstance;
};
