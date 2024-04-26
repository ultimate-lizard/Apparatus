#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "../Core/Asset.h"

class Texture : public Asset
{
public:
	Texture(const std::string& assetName, int width, int height, int bytesPerPixel, const void* pixels);
	~Texture();

	Texture() = delete;
	Texture(Texture&&) = delete;
	Texture(const Texture&) = delete;
	void operator=(const Texture&) = delete;

	virtual void init() override {}

	void bind() const;
	void unbind() const;

	const glm::ivec2& getSize() const;

private:
	std::string texturePath;
	unsigned int id;
	int bytesPerPixel;
	glm::ivec2 size;

	friend class TextureImporter;
};
