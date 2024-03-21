#include "Resource.h"

Resource::Resource(const std::string& resourceName) :
    resourceName(resourceName),
    resourceManager(nullptr)
{
    
}

std::string Resource::getResourceName() const
{
    return resourceName;
}
