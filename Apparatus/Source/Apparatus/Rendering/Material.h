#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>

#include <glm/glm.hpp>

#include "../Core/Asset.h"
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

	void createBoolParameter(const std::string& name, bool defaultValue);
	void createFloatParameter(const std::string& name, float defaultValue);
	void createVec2Parameter(const std::string& name, const glm::vec2& defaultValue);
	void createVec3Parameter(const std::string& name, const glm::vec3& defaultValue);
	void createVec4Parameter(const std::string& name, const glm::vec4& defaultValue);
	void createTextureParameter(const std::string& name, Texture* defaultValue);

	MaterialParameters& getParameters();

	Material* createInstance();

private:
	Shader* shader;
	MaterialParameters parameters;
};
