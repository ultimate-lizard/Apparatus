#pragma once

#include <memory>

#include "Importer.h"
#include "../Rendering/Texture.h"

class TextureImporter : public Importer
{
public:
	std::unique_ptr<Texture> import(const std::string& textureName, const std::string& path);
};
