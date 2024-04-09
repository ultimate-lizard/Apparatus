#pragma once

#include <string>

class Shader;

class UniformBufferObject
{
public:
	UniformBufferObject(size_t size);
	~UniformBufferObject();

	UniformBufferObject(const UniformBufferObject&) = delete;
	UniformBufferObject(UniformBufferObject&&) = delete;
	void operator=(const UniformBufferObject&) = delete;

	void createBufferBlockBinding(Shader* shader, const std::string& blockName, int bindingPoint);
	void copySubData(int offset, int size, const void* data);

	void bind();
	void unbind();

private:

	unsigned int ubo;

	Shader* shader;
	std::string blockName;
	int bindingPoint;
};
