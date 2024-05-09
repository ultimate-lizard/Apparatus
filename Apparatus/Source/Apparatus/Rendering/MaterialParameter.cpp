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

void MaterialParameters::setVec2(const std::string& name, const glm::vec2& value)
{
    auto iter = vec2Map.find(name);
    if (iter != vec2Map.end())
    {
        vec2Map[name] = value;
    }
}

glm::vec2 MaterialParameters::getVec2(const std::string& name)
{
    auto iter = vec2Map.find(name);
    if (iter != vec2Map.end())
    {
        return iter->second;
    }

    return glm::vec2(std::numeric_limits<float>::quiet_NaN());
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
    auto iter = vec4Map.find(name);
    if (iter != vec4Map.end())
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

void MaterialParameters::setTexture(const std::string& name, Texture* texture)
{
    auto iter = textureMap.find(name);
    if (iter != textureMap.end())
    {
        textureMap[name] = texture;
    }
}

Texture* MaterialParameters::getTexture(const std::string& name)
{
    auto iter = textureMap.find(name);
    if (iter != textureMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

void MaterialParameters::setTextureArray(const std::string& name, TextureArray* texture)
{
    auto iter = textureArrayMap.find(name);
    if (iter != textureArrayMap.end())
    {
        textureArrayMap[name] = texture;
    }
}

TextureArray* MaterialParameters::getTextureArray(const std::string& name)
{
    auto iter = textureArrayMap.find(name);
    if (iter != textureArrayMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

const std::map<std::string, bool>& MaterialParameters::getAllBoolParameters()
{
    return boolMap;
}

const std::map<std::string, float>& MaterialParameters::getAllFloatParameters()
{
    return floatMap;
}

const std::map<std::string, glm::vec2>& MaterialParameters::getAllVec2Parameters()
{
    return vec2Map;
}

const std::map<std::string, glm::vec3>& MaterialParameters::getAllVec3Parameters()
{
    return vec3Map;
}

const std::map<std::string, glm::vec4>& MaterialParameters::getAllVec4Parameters()
{
    return vec4Map;
}

const std::map<std::string, Texture*>& MaterialParameters::getAllTextureParameters()
{
    return textureMap;
}

const std::map<std::string, TextureArray*>& MaterialParameters::getAllTextureArrayParameters()
{
    return textureArrayMap;
}

void MaterialParameters::createBool(const std::string& name, bool defaultValue)
{
    boolMap.emplace(name, defaultValue);
}

void MaterialParameters::createFloat(const std::string& name, float defaultValue)
{
    floatMap.emplace(name, defaultValue);
}

void MaterialParameters::createVec2(const std::string& name, const glm::vec2& defaultValue)
{
    vec2Map.emplace(name, defaultValue);
}

void MaterialParameters::createVec3(const std::string& name, const glm::vec3& defaultValue)
{
    vec3Map.emplace(name, defaultValue);
}

void MaterialParameters::createVec4(const std::string& name, const glm::vec4& defaultValue)
{
    vec4Map.emplace(name, defaultValue);
}

void MaterialParameters::createTexture(const std::string& name, Texture* defaultValue)
{
    textureMap.emplace(name, defaultValue);
}

void MaterialParameters::createTextureArray(const std::string& name, TextureArray* defaultValue)
{
    textureArrayMap.emplace(name, defaultValue);
}
