#pragma once

#include "Component.h"
#include "../Rendering/SceneNode.h"
#include "../Rendering/Camera.h"

class CameraComponent : public Component, public SceneNode
{
public:
	CameraComponent();
	CameraComponent(const std::string& componentName);
	CameraComponent(const CameraComponent& other);

	CameraComponent(CameraComponent&&) = delete;
	void operator=(const CameraComponent&) = delete;

	virtual std::unique_ptr<Component> clone() override;

	virtual void init() override;

	Camera& getCamera();

private:
	Camera camera;
};
