#include "Material.h"

#include <assimp/material.h>

#include "../Core/Logger.h"
#include "../Core/ResourceManager.h"
#include "Shader.h"
#include "Texture.h"

Material::Material(const std::string& resourceName) :
	Resource("Material_" + resourceName),
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
			tex->bind();
		}
	}
}

void Material::addDiffuseTexture(Texture* texture)
{
	diffuseMaps.push_back(texture);
}

std::unique_ptr<MaterialInstance> Material::createMaterialInstance()
{
	auto newInstance = std::make_unique<MaterialInstance>();
	newInstance->material = this;

	return newInstance;
}
