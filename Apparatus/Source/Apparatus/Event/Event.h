#pragma once

class LightComponent;

class Event
{
public:
	virtual ~Event() = default;
};

class LightComponentCreationEvent : public Event
{
public:
	LightComponentCreationEvent(LightComponent* lightComponent);
	virtual ~LightComponentCreationEvent() = default;
	LightComponent* getLightComponent();

	LightComponentCreationEvent() = delete;
	LightComponentCreationEvent(const LightComponentCreationEvent&) = delete;
	LightComponentCreationEvent(LightComponentCreationEvent&&) = delete;

private:
	LightComponent* lightComponent = nullptr;
};
