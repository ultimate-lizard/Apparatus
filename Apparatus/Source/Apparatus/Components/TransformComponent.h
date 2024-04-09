#pragma once

#include "Component.h"
#include "../Rendering/SceneNode.h"

class Level;

class TransformComponent : public Component, public SceneNode
{
public:
	TransformComponent();
	TransformComponent(const std::string& componentName);
	TransformComponent(const TransformComponent& other);

	virtual ~TransformComponent() = default;
	TransformComponent(TransformComponent&&) = delete;
	void operator=(const TransformComponent&) = delete;

	virtual std::unique_ptr<Component> clone() override;

	virtual void update(float dt) override;
};
