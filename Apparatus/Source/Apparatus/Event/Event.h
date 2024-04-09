#pragma once

class PointLightComponent;

class Event
{
public:
	virtual ~Event() = default;
};

class LightComponentCreationEvent : public Event
{
public:
	PointLightComponent* pointLightComponent = nullptr;
};
