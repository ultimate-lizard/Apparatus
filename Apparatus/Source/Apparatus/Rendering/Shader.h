#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../Core/Resource.h"

class Shader : public Resource
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

	void setUniform(const std::string& name, const glm::mat4& matrix) const;
	void setUniform(const std::string& name, const glm::vec3& vector) const; 
	void setUniform(const std::string& name, const glm::vec4& vector) const; 
	void setUniform(const std::string& name, bool value) const;

private:
	std::string loadFile(const std::string& path);

	std::string vertPath;
	std::string fragPath;

	unsigned int program;
};
