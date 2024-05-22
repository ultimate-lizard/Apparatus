#include "TextureImporter.h"

#include <SDL2/SDL_image.h>

#include "../Core/Logger.h"

std::unique_ptr<Texture> TextureImporter::import(const std::string& path)
{
	std::unique_ptr<Texture> newTexture;

	if (SDL_Surface* img = IMG_Load(path.c_str()))
	{
		if (newTexture = std::make_unique<Texture>(img->w, img->h, img->format->BytesPerPixel, img->pixels))
		{
			newTexture->texturePath = path;
		}

		SDL_FreeSurface(img);
	}
	else
	{
		LOG("Couldn't load a texture from path '" + path + "'", LogLevel::Error);
	}

    return newTexture;
}
