#pragma once

#include <bitset>

#include "MovementComponent.h"
#include "../Rendering/Rotator.h"

class DebugCameraMovementComponent : public MovementComponent
{
public:
	DebugCameraMovementComponent(const std::string& id);

	virtual void update(float dt) override;

	void rotateRoll(float rate);

private:
	int roll;
};
