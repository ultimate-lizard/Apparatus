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

void MaterialParameters::setFloat(const std::string& name, float value)
{
    auto iter = floatMap.find(name);
    if (iter != floatMap.end())
    {
        floatMap[name] = value;
    }
}

float MaterialParameters::getFloat(const std::string& name)
{
    auto iter = floatMap.find(name);
    if (iter != floatMap.end())
    {
        return iter->second;
    }

    return false;
}

void MaterialParameters::setVec3(const std::string& name, const glm::vec3& value)
{
    auto iter = vec3Map.find(name);
    if (iter != vec3Map.end())
    {
        vec3Map[name] = value;
    }
}

glm::vec3 MaterialParameters::getVec3(const std::string& name)
{
    auto iter = vec3Map.find(name);
    if (iter != vec3Map.end())
    {
        return iter->second;
    }

    return glm::vec3(std::numeric_limits<float>::quiet_NaN());
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

const std::map<std::string, float>& MaterialParameters::getAllFloatParameters()
{
    return floatMap;
}

const std::map<std::string, glm::vec3>& MaterialParameters::getAllVec3Parameters()
{
    return vec3Map;
}

const std::map<std::string, glm::vec4>& MaterialParameters::getAllVec4Parameters()
{
    return vec4Map;
}

void MaterialParameters::createBool(const std::string& name, bool defaultValue)
{
    boolMap.emplace(name, defaultValue);
}

void MaterialParameters::createFloat(const std::string& name, float defaultValue)
{
    floatMap.emplace(name, defaultValue);
}

void MaterialParameters::createVec3(const std::string& name, const glm::vec3& defaultValue)
{
    vec3Map.emplace(name, defaultValue);
}

void MaterialParameters::createVec4(const std::string& name, const glm::vec4& defaultValue)
{
    vec4Map.emplace(name, defaultValue);
}
