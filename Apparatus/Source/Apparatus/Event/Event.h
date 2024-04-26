#pragma once

#include <glm/glm.hpp>

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

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(const glm::ivec2& windowSize);

	const glm::ivec2& getWindowSize() const;

private:
	glm::ivec2 windowSize;
};
