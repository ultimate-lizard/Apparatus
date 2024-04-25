#include "Texture.h"


#include <glad/glad.h>



Texture::Texture(const std::string& assetName, int width, int height, int bytesPerPixel, const void* pixels) :
	Asset(assetName),
	id(0)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	int format = bytesPerPixel == 3 ? GL_RGB : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}
