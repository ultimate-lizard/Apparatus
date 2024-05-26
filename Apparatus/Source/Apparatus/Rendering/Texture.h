#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "../Core/Asset.h"

enum class TextureFiltering
{
	Nearest,
	Linear,
	MipNearest,
	MipLinear
};

class Texture : public Asset
{
public:
	Texture(int width, int height, int bytesPerPixel, const void* pixels);
	~Texture();

	Texture() = delete;
	Texture(Texture&&) = delete;
	Texture(const Texture&) = delete;
	void operator=(const Texture&) = delete;

	virtual void init() override {}

	void bind() const;
	void unbind() const;

	const glm::ivec2& getSize() const;

	void setMagFilter(TextureFiltering filtering);
	void setMinFilter(TextureFiltering filtering);

private:
	int getGlFiltering(TextureFiltering filtering);

	std::string texturePath;
	unsigned int id;
	int bytesPerPixel;
	glm::ivec2 size;

	friend class TextureImporter;
};
