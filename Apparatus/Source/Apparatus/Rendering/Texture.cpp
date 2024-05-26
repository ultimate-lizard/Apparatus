#include "Texture.h"

#include <glad/glad.h>

Texture::Texture(int width, int height, int bytesPerPixel, const void* pixels) :
	id(0),
	bytesPerPixel(bytesPerPixel)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int format = bytesPerPixel == 3 ? GL_RGB : GL_RGBA;
	int internalFormat = bytesPerPixel == 3 ? GL_RGB8 : GL_RGBA8;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	size.x = width;
	size.y = height;

	unbind();
}

Texture::~Texture()
{
	unbind();
	glDeleteTextures(1, &id);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

const glm::ivec2& Texture::getSize() const
{
	return size;
}

void Texture::setMagFilter(TextureFiltering filtering)
{
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGlFiltering(filtering));
	glGenerateMipmap(GL_TEXTURE_2D);

	unbind();
}

void Texture::setMinFilter(TextureFiltering filtering)
{
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGlFiltering(filtering));
	glGenerateMipmap(GL_TEXTURE_2D);

	unbind();
}

int Texture::getGlFiltering(TextureFiltering filtering)
{
	int glFiltering = GL_NEAREST;

	switch (filtering)
	{
	case TextureFiltering::Linear:
		glFiltering = GL_LINEAR;
		break;
	case TextureFiltering::MipNearest:
		glFiltering = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case TextureFiltering::MipLinear:
		glFiltering = GL_LINEAR_MIPMAP_LINEAR;
		break;
	}

	return glFiltering;
}
