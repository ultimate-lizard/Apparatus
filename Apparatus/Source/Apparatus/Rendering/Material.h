#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>

#include <glm/glm.hpp>

#include "../Core/Asset.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

class Shader;
class Texture;

class Material : public Asset
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
	const std::vector<Texture*>& getDiffuseMaps() const;

	std::unique_ptr<MaterialInstance> createMaterialInstance();

	void createBoolParameter(const std::string& name, bool defaultValue);
	void createFloatParameter(const std::string& name, float defaultValue);
	void createVec3Parameter(const std::string& name, const glm::vec3& defaultValue);
	void createVec4Parameter(const std::string& name, const glm::vec4& defaultValue);

private:
	Shader* shader;
	std::vector<Texture*> diffuseMaps;
	MaterialParameters parameters;
};
