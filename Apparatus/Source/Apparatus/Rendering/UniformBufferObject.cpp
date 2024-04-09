#include "UniformBufferObject.h"

#include <glad/glad.h>

#include "../Rendering/Shader.h"

UniformBufferObject::UniformBufferObject(size_t size) :
	ubo(0),
	shader(nullptr),
	bindingPoint(0)
{
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBufferObject::~UniformBufferObject()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDeleteBuffers(1, &ubo);
}

void UniformBufferObject::createBufferBlockBinding(Shader* shader, const std::string& blockName, int bindingPoint)
{
	if (shader)
	{
		unsigned int index = glGetUniformBlockIndex(shader->getProgram(), blockName.c_str());
		glUniformBlockBinding(shader->getProgram(), index, bindingPoint);
	}
}

void UniformBufferObject::copySubData(int offset, int size, const void* data)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBufferObject::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
}

void UniformBufferObject::unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
