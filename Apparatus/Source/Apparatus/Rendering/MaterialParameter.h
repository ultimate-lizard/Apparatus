#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

class MaterialParameters
{
public:
	void setBool(const std::string& name, bool value);
	bool getBool(const std::string& name);

	void setVec4(const std::string& name, const glm::vec4& value);
	glm::vec4 getVec4(const std::string& name);

	const std::map<std::string, bool>& getAllBoolParameters();
	const std::map<std::string, glm::vec4>& getAllVec4Parameters();

protected:
	void createBool(const std::string& name, bool defaultValue);
	void createVec4(const std::string& name, const glm::vec4& defaultValue);

	std::map<std::string, bool> boolMap;
	std::map<std::string, glm::vec4> vec4Map;

	friend class Material;
};
