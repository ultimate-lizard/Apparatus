#include "Material.h"

#include <assimp/material.h>

#include "../Core/Logger.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Texture.h"

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

void Material::bind() const
{
	for (Texture* tex : diffuseMaps)
	{
		if (tex)
		{
			// TODO: glActiveTexture(GL_TEXTURE + i);
			tex->bind();
		}
	}
}

void Material::addDiffuseTexture(Texture* texture)
{
	diffuseMaps.push_back(texture);
}

const std::vector<Texture*>& Material::getDiffuseMaps() const
{
	return diffuseMaps;
}

std::unique_ptr<MaterialInstance> Material::createMaterialInstance()
{
	auto newInstance = std::make_unique<MaterialInstance>();
	newInstance->material = this;
	newInstance->parameters = parameters;

	return newInstance;
}

void Material::createBoolParameter(const std::string& name, bool defaultValue)
{
	parameters.createBool(name, defaultValue);
}

void Material::createFloatParameter(const std::string& name, float defaultValue)
{
	parameters.createFloat(name, defaultValue);
}

void Material::createVec3Parameter(const std::string& name, const glm::vec3& defaultValue)
{
	parameters.createVec3(name, defaultValue);
}

void Material::createVec4Parameter(const std::string& name, const glm::vec4& defaultValue)
{
	parameters.createVec4(name, defaultValue);
}
