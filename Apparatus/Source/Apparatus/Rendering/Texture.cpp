#include "Texture.h"

#include <SDL2/SDL_image.h>
#include <glad/glad.h>

#include "../Core/Logger.h"

Texture::Texture(const std::string& resourceName, const std::string& texturePath) :
	Asset(resourceName),
	texturePath(texturePath),
	id(0)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::init()
{
	if (SDL_Surface* img = IMG_Load(texturePath.c_str()))
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// TODO: Variable bits per pixel
		int format = img->format->BytesPerPixel == 3 ? GL_RGB : GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, img->w, img->h, 0, format, GL_UNSIGNED_BYTE, img->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		SDL_FreeSurface(img);
	}
	else
	{
		LOG("Couldn't load a texture from path '" + texturePath + "'", LogLevel::Error);
	}
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}
