#pragma once

#include "Component.h"
#include "../Rendering/Rotator.h"

class MovementComponent : public Component
{
public:
	MovementComponent();
	MovementComponent(const std::string& componentName);
	MovementComponent(const MovementComponent& other);

	virtual std::unique_ptr<Component> clone() override;

	virtual void init() override;
	virtual void update(float dt) override;

	void moveZ(float rate);
	void moveX(float rate);

	void lookX(float rate);
	void lookY(float rate);

protected:
	int forward;
	int sideways;

	Rotator headRotator;
};
