#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

class Texture;
class TextureArray;

class MaterialParameters
{
public:
	void setBool(const std::string& name, bool value);
	bool getBool(const std::string& name);

	void setFloat(const std::string& name, float value);
	float getFloat(const std::string& name);

	void setVec2(const std::string& name, const glm::vec2& value);
	glm::vec2 getVec2(const std::string& name);

	void setVec3(const std::string& name, const glm::vec3& value);
	glm::vec3 getVec3(const std::string& name);

	void setVec4(const std::string& name, const glm::vec4& value);
	glm::vec4 getVec4(const std::string& name);

	void setTexture(const std::string& name, Texture* texture);
	Texture* getTexture(const std::string& name);

	void setTextureArray(const std::string& name, TextureArray* texture);
	TextureArray* getTextureArray(const std::string& name);

	const std::map<std::string, bool>& getAllBoolParameters();
	const std::map<std::string, float>& getAllFloatParameters();
	const std::map<std::string, glm::vec2>& getAllVec2Parameters();
	const std::map<std::string, glm::vec3>& getAllVec3Parameters();
	const std::map<std::string, glm::vec4>& getAllVec4Parameters();
	const std::map<std::string, Texture*>& getAllTextureParameters();
	const std::map<std::string, TextureArray*>& getAllTextureArrayParameters();

protected:
	void createBool(const std::string& name, bool defaultValue);
	void createFloat(const std::string& name, float defaultValue);
	void createVec2(const std::string& name, const glm::vec2& defaultValue);
	void createVec3(const std::string& name, const glm::vec3& defaultValue);
	void createVec4(const std::string& name, const glm::vec4& defaultValue);
	void createTexture(const std::string& name, Texture* defaultValue);
	void createTextureArray(const std::string& name, TextureArray* defaultValue);

	std::map<std::string, bool> boolMap;
	std::map<std::string, float> floatMap;
	std::map<std::string, glm::vec2> vec2Map;
	std::map<std::string, glm::vec3> vec3Map;
	std::map<std::string, glm::vec4> vec4Map;
	std::map<std::string, Texture*> textureMap;
	std::map<std::string, TextureArray*> textureArrayMap;

	friend class Material;
};
