#pragma once

#include "Component.h"
#include "../Rendering/SceneNode.h"
#include "../Rendering/ModelInstance.h"
#include "../Util/Primitive.h"

class TransformComponent : public Component, public SceneNode
{
public:
	TransformComponent(Entity* owner);

	virtual void init() override;
	virtual void update(float dt) override;

	void setDebugModel(Model* model);
	Model* getDebugModel();
	ModelInstance* getDebugModelInstance();

protected:
	virtual void assignDefaultObjectName() override;

	Model* debugModel;
	std::unique_ptr<ModelInstance> debugModelInstance;
};
