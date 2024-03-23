#pragma once

#include "Component.h"
#include "../Rendering/Camera.h"

class CameraComponent : public Component
{
public:
	CameraComponent(Entity* owner);

	Camera& getCamera();

protected:
	virtual void assignDefaultObjectName() override;

private:
	Camera camera;
};
