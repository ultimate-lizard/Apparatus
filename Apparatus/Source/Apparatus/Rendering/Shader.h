#pragma once

#include <string>
#include <map>
#include <memory>

#include <glm/glm.hpp>

#include "../Core/Asset.h"
#include "UniformBufferObject.h"

class Shader : public Asset
{
public:
	Shader(const std::string& resourceName, const std::string& vertPath, const std::string& fragPath);
	~Shader();

	Shader() = delete;
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	void operator=(const Shader&) = delete;

	virtual void init() override;

	void bind() const;

	void setUniform(const std::string& name, const glm::mat4& matrix);
	void setUniform(const std::string& name, const glm::vec2& vector); 
	void setUniform(const std::string& name, const glm::vec3& vector); 
	void setUniform(const std::string& name, const glm::vec4& vector); 
	void setUniform(const std::string& name, bool value);
	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, int value);

	unsigned int getProgram() const;

	UniformBufferObject* createUniformBufferObject(size_t size, const std::string& blockName, int bindingPoint);
	UniformBufferObject* findUniformBufferObject(const std::string& blockName);

private:
	std::string loadFile(const std::string& path);

	std::string vertPath;
	std::string fragPath;

	unsigned int program;

	std::map<std::string, std::unique_ptr<UniformBufferObject>> ubos;
};
