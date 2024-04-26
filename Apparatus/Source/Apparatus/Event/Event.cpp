#include "Event.h"

LightComponentCreationEvent::LightComponentCreationEvent(LightComponent* lightComponent) :
    lightComponent(lightComponent)
{
}

LightComponent* LightComponentCreationEvent::getLightComponent()
{
    return lightComponent;
}

WindowResizeEvent::WindowResizeEvent(const glm::ivec2& windowSize) :
    windowSize(windowSize)
{

}

const glm::ivec2& WindowResizeEvent::getWindowSize() const
{
    return windowSize;
}
