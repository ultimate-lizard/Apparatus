#include "TextureArray.h"

#include <glad/glad.h>

TextureArray::TextureArray(const std::string& assetName, int width, int height, int depth, int bytesPerPixel) :
	Asset(assetName),
	id(0),
	bytesPerPixel(bytesPerPixel)
{
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);

	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int format = bytesPerPixel == 3 ? GL_RGB : GL_RGBA;
	// int internalFormat = bytesPerPixel == 3 ? GL_RGB8 : GL_RGBA8;

	const int mipLevelCount = 1;
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, width, height, depth, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
	// glGenerateMipmap(GL_TEXTURE_2D);

	size.x = width;
	size.y = height;

	unbind();
}

TextureArray::~TextureArray()
{
	unbind();
	glDeleteTextures(1, &id);
}

void TextureArray::bind() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);
}

void TextureArray::unbind() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

const glm::ivec2& TextureArray::getSize() const
{
	return size;
}

void TextureArray::copySubData(int width, int height, int depth, int layerCount, int format, const void* data)
{
	bind();
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, depth, width, height, layerCount, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unbind();
}
