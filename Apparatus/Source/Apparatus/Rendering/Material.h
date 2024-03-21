#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "../Core/Resource.h"
#include "MaterialInstance.h"

class Shader;
class Texture;

class Material : public Resource
{
public:
	Material(const std::string& resourceName);
	~Material() = default;
	
	Material(const Material&) = delete;
	Material(Material&&) = delete;
	void operator=(const Material&) = delete;

	void setShader(Shader* shader);
	Shader* getShader();

	virtual void init() override {}

	void bind() const;

	void addDiffuseTexture(Texture* texture);

	std::unique_ptr<MaterialInstance> createMaterialInstance();

private:
	Shader* shader;
	std::vector<Texture*> diffuseMaps;
};
