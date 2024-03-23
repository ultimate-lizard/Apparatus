#pragma once

#include <string>
#include <map>

#include <glm/glm.hpp>

#include "MaterialParameter.h"

class Material;

class MaterialInstance
{
public:
	MaterialInstance();
	~MaterialInstance() = default;

	MaterialInstance(const MaterialInstance&) = delete;
	MaterialInstance(MaterialInstance&&) = delete;
	void operator=(const MaterialInstance&) = delete;

	Material* getMaterial();

	MaterialParameters& getMaterialParameters();

private:
	Material* material;
	MaterialParameters parameters;

	friend class Material;
};
