#pragma once

#include "Component.h"
#include "../Rendering/Camera.h"

class CameraComponent : public Component
{
public:
	CameraComponent(const std::string& id);

	virtual void init() override;
	virtual void update(float dt) override;

	Camera& getCamera();

private:
	Camera camera;
};
