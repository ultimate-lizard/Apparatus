#pragma once

#include <bitset>

#include "MovementComponent.h"
#include "../Rendering/Rotator.h"

class DebugCameraMovementComponent : public MovementComponent
{
public:
	DebugCameraMovementComponent(Entity* owner);

	virtual void update(float dt) override;

	void rotateRoll(float rate);

protected:
	virtual void assignDefaultObjectName() override;

	int roll;
};
