#pragma once

#include "Component.h"
#include "../Rendering/Rotator.h"

class MovementComponent : public Component
{
public:
	MovementComponent(Entity* owner);

	virtual void init() override;
	virtual void update(float dt) override;

	void moveZ(float rate);
	void moveX(float rate);

	void lookX(float rate);
	void lookY(float rate);

protected:
	virtual void assignDefaultObjectName() override;

	int forward;
	int sideways;

	Rotator headRotator;
};
