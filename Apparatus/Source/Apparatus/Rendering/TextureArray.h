#pragma once

#include <glm/glm.hpp>

#include "../Core/Asset.h"

class TextureArray : public Asset
{
public:
	TextureArray(int width, int height, int depth, int bytesPerPixel);
	~TextureArray();

	TextureArray() = delete;
	TextureArray(TextureArray&&) = delete;
	TextureArray(const TextureArray&) = delete;
	void operator=(const TextureArray&) = delete;

	virtual void init() override {}

	void bind() const;
	void unbind() const;

	const glm::ivec2& getSize() const;

	void copySubData(int width, int height, int depth, int layerCount, int format, const void* data);

private:
	std::string texturePath;
	unsigned int id;
	int bytesPerPixel;
	glm::ivec2 size;

	friend class TextureImporter;
};
