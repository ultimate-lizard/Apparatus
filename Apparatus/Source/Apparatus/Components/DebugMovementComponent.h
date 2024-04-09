#pragma once

#include <bitset>

#include "MovementComponent.h"
#include "../Rendering/Rotator.h"

class DebugCameraMovementComponent : public MovementComponent
{
public:
	DebugCameraMovementComponent();
	DebugCameraMovementComponent(const std::string& componentName);

	virtual void update(float dt) override;

	void rotateRoll(float rate);

protected:
	int roll;
};
