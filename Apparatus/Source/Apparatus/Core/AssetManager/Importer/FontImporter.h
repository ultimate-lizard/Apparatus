#pragma once

#include <memory>
#include <string>

#include <freetype/freetype.h>

#include "Importer.h"
#include "../../../UI/Font.h"

class FontImporter : public Importer
{
public:
	virtual void init() override;
	virtual void uninit() override;

	std::unique_ptr<Font> import(const std::string& path);

private:
	FT_Library ft;
};
