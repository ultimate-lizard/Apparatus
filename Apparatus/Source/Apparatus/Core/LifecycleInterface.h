#pragma once

class LifecycleInterface
{
public:
	virtual void init() = 0;
	virtual void start() = 0;
	virtual void update(float dt) = 0;
};
