#pragma once

#include <glm/glm.hpp>

class Material;

class MaterialInstance
{
public:
	MaterialInstance() = default;
	~MaterialInstance() = default;

	MaterialInstance(const MaterialInstance&) = delete;
	MaterialInstance(MaterialInstance&&) = delete;
	void operator=(const MaterialInstance&) = delete;

	glm::vec4 color = {};
	bool selected = false;

	Material* getMaterial();

private:
	Material* material;

	friend class Material;
};
