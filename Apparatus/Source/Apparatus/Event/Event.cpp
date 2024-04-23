#include "Event.h"

LightComponentCreationEvent::LightComponentCreationEvent(LightComponent* lightComponent) :
    lightComponent(lightComponent)
{
}

LightComponent* LightComponentCreationEvent::getLightComponent()
{
    return lightComponent;
}
