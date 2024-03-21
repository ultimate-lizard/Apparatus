#pragma once

#include "Component.h"
#include "../Rendering/SceneNode.h"
#include "../Rendering/ModelInstance.h"
#include "../Common/Primitives.h"

class TransformComponent : public Component, public SceneNode
{
public:
	TransformComponent(const std::string& id);

	virtual void init() override;
	virtual void update(float dt) override;

	void setDebugModel(Model* model);
	Model* getDebugModel();
	ModelInstance* getDebugModelInstance();

protected:
	Model* debugModel;
	std::unique_ptr<ModelInstance> debugModelInstance;
};
