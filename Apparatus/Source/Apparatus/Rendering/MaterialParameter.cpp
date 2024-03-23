#include "MaterialParameter.h"

void MaterialParameters::setBool(const std::string& name, bool value)
{
    auto iter = boolMap.find(name);
    if (iter != boolMap.end())
    {
        boolMap[name] = value;
    }
}

bool MaterialParameters::getBool(const std::string& name)
{
    auto iter = boolMap.find(name);
    if (iter != boolMap.end())
    {
        return iter->second;
    }

    return false;
}

void MaterialParameters::setVec4(const std::string& name, const glm::vec4& value)
{
    auto iter = boolMap.find(name);
    if (iter != boolMap.end())
    {
        vec4Map[name] = value;
    }
}

glm::vec4 MaterialParameters::getVec4(const std::string& name)
{
    auto iter = vec4Map.find(name);
    if (iter != vec4Map.end())
    {
        return iter->second;
    }

    return glm::vec4(std::numeric_limits<float>::quiet_NaN());
}

const std::map<std::string, bool>& MaterialParameters::getAllBoolParameters()
{
    return boolMap;
}

const std::map<std::string, glm::vec4>& MaterialParameters::getAllVec4Parameters()
{
    return vec4Map;
}

void MaterialParameters::createBool(const std::string& name, bool defaultValue)
{
    boolMap.emplace(name, defaultValue);
}

void MaterialParameters::createVec4(const std::string& name, const glm::vec4& defaultValue)
{
    vec4Map.emplace(name, defaultValue);
}
