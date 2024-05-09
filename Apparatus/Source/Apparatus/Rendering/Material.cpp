#include "Material.h"

#include <assimp/material.h>
#include <string>

#include "../Core/Logger.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureArray.h"
#include "../Apparatus.h"

static std::map<std::string, size_t> instanceCount;

Material::Material(const std::string& resourceName) :
	Asset(resourceName),
	shader(nullptr)
{
}

void Material::setShader(Shader* shader)
{
	this->shader = shader;
}

Shader* Material::getShader()
{
	return shader;
}

void Material::createBoolParameter(const std::string& name, bool defaultValue)
{
	parameters.createBool(name, defaultValue);
}

void Material::createFloatParameter(const std::string& name, float defaultValue)
{
	parameters.createFloat(name, defaultValue);
}

void Material::createVec2Parameter(const std::string& name, const glm::vec2& defaultValue)
{
	parameters.createVec2(name, defaultValue);
}

void Material::createVec3Parameter(const std::string& name, const glm::vec3& defaultValue)
{
	parameters.createVec3(name, defaultValue);
}

void Material::createVec4Parameter(const std::string& name, const glm::vec4& defaultValue)
{
	parameters.createVec4(name, defaultValue);
}

void Material::createTextureParameter(const std::string& name, Texture* defaultValue)
{
	parameters.createTexture(name, defaultValue);
}

void Material::createTextureArrayParameter(const std::string& name, TextureArray* defaultValue)
{
    parameters.createTextureArray(name, defaultValue);
}

MaterialParameters& Material::getParameters()
{
	return parameters;
}

Material* Material::createInstance()
{
	std::string newName = getAssetName() + "_Instance_" + std::to_string(instanceCount[getAssetName()]);

	Material* newMaterial = Apparatus::getAssetManager().createAsset<Material>(newName);
	newMaterial->parameters = parameters;
	newMaterial->shader = shader;

	instanceCount[getAssetName()] += 1;

	return newMaterial;
}

void Material::submitUniforms()
{
    if (!shader)
    {
        return;
    }

    shader->bind();

    MaterialParameters& params = getParameters();

    for (auto mapIter : params.getAllBoolParameters())
    {
        shader->setUniform(mapIter.first, mapIter.second);
    }

    for (auto mapIter : params.getAllFloatParameters())
    {
        shader->setUniform(mapIter.first, mapIter.second);
    }

    for (auto mapIter : params.getAllVec2Parameters())
    {
        shader->setUniform(mapIter.first, mapIter.second);
    }

    for (auto mapIter : params.getAllVec3Parameters())
    {
        shader->setUniform(mapIter.first, mapIter.second);
    }

    for (auto mapIter : params.getAllVec4Parameters())
    {
        shader->setUniform(mapIter.first, mapIter.second);
    }

    int textureIndex = 0;
    for (auto mapIter : params.getAllTextureParameters())
    {
        if (Texture* texture = mapIter.second)
        {
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            texture->bind();
            textureIndex++;
        }
    }

    int textureAtlasIndex = 0;
    for (auto mapIter : params.getAllTextureArrayParameters())
    {
        if (TextureArray* texture = mapIter.second)
        {
            // glActiveTexture(GL_TEXTURE0 + textureAtlasIndex);
            texture->bind();
            textureAtlasIndex++;
        }
    }
}
