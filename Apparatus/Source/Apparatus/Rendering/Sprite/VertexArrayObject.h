#pragma once

#include <vector>

#include <glad/glad.h>

class VertexArrayObject
{
public:
	enum class VertexAttributeType
	{
		Float
	};

	struct VertexAttribute
	{
		int size = 0;
		VertexAttributeType type = VertexAttributeType::Float;
		bool normalized = false;
		int strideFactor = 0;
	};

	VertexArrayObject()
	{
		glCreateVertexArrays(1, &vao);
	}

	void setStride(int stride)
	{
		this->stride = stride;
	}

	void addAttribute(int size, VertexAttributeType type = VertexAttributeType::Float, bool normalized = false)
	{
		VertexAttribute attribute;
		attribute.size = size;
		attribute.type = type;
		attribute.normalized = normalized;
		attributes.push_back(attribute);
	}

	void bind() const
	{
		glBindVertexArray(vao);
	}

	void unbind() const
	{
		glBindVertexArray(0);
	}

	void finalize()
	{
		bind();

		int pointer = 0;

		for (int i = 0; i < attributes.size(); ++i)
		{
			VertexAttribute& attribute = attributes[i];

			int type = GL_FLOAT;

			switch (attribute.type)
			{
			case VertexAttributeType::Float:
				type = GL_FLOAT;
				break;
			}

			glVertexAttribPointer(i, attribute.size, type, attribute.normalized, stride, reinterpret_cast<void*>(static_cast<int64_t>(pointer * sizeof(float))));
			glEnableVertexAttribArray(i);

			pointer += attribute.size;
		}

		unbind();
	}

private:
	unsigned int vao;
	std::vector<VertexAttribute> attributes;
	int stride = 0;
};

//class SpriteMesh
//{
//public:
//	SpriteMesh(size_t size);
//
//	void setSubData(const std::vector<float>& vertices);
//	void bind();
//
//	const std::vector<float>& getVertices() const;
//
//private:
//	unsigned int vbo;
//	VertexArrayObject vao;
//
//	std::vector<float> vertices;
//};
