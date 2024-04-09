#include "Shader.h"

#include <fstream>
#include <sstream>

#include <glad/glad.h>

Shader::Shader(const std::string& resourceName, const std::string& vertPath, const std::string& fragPath) :
	Asset(resourceName),
	vertPath(vertPath),
	fragPath(fragPath),
	program(0)
{
}

void Shader::init()
{
	std::string vertSrc = loadFile(vertPath);
	std::string fragSrc = loadFile(fragPath);

	const char* vertCsrc = vertSrc.c_str();

	unsigned int vert = 0;
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertCsrc, nullptr);
	glCompileShader(vert);

	const char* fragCsrc = fragSrc.c_str();

	unsigned int frag = 0;
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragCsrc, nullptr);
	glCompileShader(frag);

	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::bind() const
{
	glUseProgram(program);
}

void Shader::setUniform(const std::string& name, const glm::mat4& matrix) const
{
	unsigned int location = glGetUniformLocation(program, name.c_str());
	glUniformMatrix4fv(location, 1, false, &matrix[0][0]);
}

void Shader::setUniform(const std::string& name, const glm::vec3& vector) const
{
	unsigned int location = glGetUniformLocation(program, name.c_str());
	glUniform3fv(location, 1, &vector[0]);
}

void Shader::setUniform(const std::string& name, const glm::vec4& vector) const
{
	unsigned int location = glGetUniformLocation(program, name.c_str());
	glUniform4fv(location, 1, &vector[0]);
}

void Shader::setUniform(const std::string& name, bool value) const
{
	unsigned int location = glGetUniformLocation(program, name.c_str());
	glUniform1i(location, value);
}

void Shader::setUniform(const std::string& name, float value) const
{
	unsigned int location = glGetUniformLocation(program, name.c_str());
	glUniform1f(location, value);
}

void Shader::setUniform(const std::string& name, int value) const
{
	unsigned int location = glGetUniformLocation(program, name.c_str());
	glUniform1i(location, value);
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

std::string Shader::loadFile(const std::string& path)
{
	std::string result;

	std::fstream file(path);
	if (file.is_open())
	{
		std::stringstream ss;
		ss << file.rdbuf();

		result = ss.str();
	}

	return result;
}
